#include <thread>
#include <numeric>
#include <vector>
#include <atomic>
#include <algorithm>
#include "changecolor.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "opencv2/opencv.hpp"

namespace  CHANGE_COLOR {
	struct Double4
	{
		Double4() : Double4(0) {}

		Double4(double r, double g, double b)
			: r(r),
			g(g),
			b(b),
			a(1.f) {}

		Double4(double f) : r(f), g(f), b(f), a(f) {}

		Double4(double r, double g, double b, double a)
			: r(r),
			g(g),
			b(b),
			a(a) {}

		Double4 operator-(const Double4& rhs) {
			return Double4{ r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a };
		}

		Double4 operator/(const Double4& rhs) {
			return Double4{ r / rhs.r, g / rhs.g, b / rhs.b, a / rhs.a };
		}
		double r, g, b, a;
	};



	Double4 RGBToHSV(const Double4& rgb) {
		double cMax, cMin, h, s, v, cDelta;
		Double4 c;

		cMax = std::max(rgb.r, std::max(rgb.g, rgb.b));
		cMin = std::min(rgb.r, std::min(rgb.g, rgb.b));
		cDelta = cMax - cMin;

		v = cMax;
		if (cMax != 0.0) {
			s = cDelta / cMax;
		}
		else {
			s = 0.0;
			h = 0.0;
		}

		if (s == 0.0) {
			h = 0.0;
		}
		else {
			c = (Double4(cMax) - rgb);
			c = c / Double4(cDelta);
			if (rgb.r == cMax) {
				h = c.b - c.g;
			}
			else if (rgb.g == cMax) {
				h = 2.0f + c.r - c.b;
			}
			else {
				h = 4.0f + c.g - c.r;
			}

			h /= 6.0f;

			if (h < 0.0) {
				h += 1.0;
			}
		}
		return Double4{ h, s, v, rgb.a };
	}

	Double4 HSVToRGB(const Double4& hsv) {
		double i, f, p, q, t, h, s, v;
		Double4 rgb;

		h = hsv.r;
		s = hsv.g;
		v = hsv.b;

		if (s == 0.0) {
			rgb = Double4(v, v, v, 1);
		}
		else {
			if (h == 1.0) {
				h = 0.0;
			}

			h *= 6.0;
			i = floor(h);
			f = h - i;
			rgb = Double4(f, f, f, 1);
			p = v * (1.0f - s);
			q = v * (1.0f - (s * f));
			t = v * (1.0f - (s * (1.0f - f)));

			if (i == 0.0) {
				rgb = Double4(v, t, p);
			}
			else if (i == 1.0) {
				rgb = Double4(q, v, p);
			}
			else if (i == 2.0) {
				rgb = Double4(p, v, t);
			}
			else if (i == 3.0) {
				rgb = Double4(p, q, v);
			}
			else if (i == 4.0) {
				rgb = Double4(t, p, v);
			}
			else {
				rgb = Double4(v, p, q);
			}
		}
		return Double4{ rgb.r, rgb.g, rgb.b, hsv.a };
	}

	double clamp(double input, double lower, double upper) {
		return std::max(std::min(input, upper), lower);
	}

	Result changeColor(unsigned char* data, int picWidth, int picHeight, int picChannel) {
		auto pixelCount = picWidth * picHeight;
		// int imageDataLengthInChar = getPixelCount() * getPixelLength(mResultType);
		// int* imageData = new int[imageDataLengthInChar / sizeof(int)];

		auto toOpenCVFormatHSV = [](const Double4& hsv) -> Double4 {
			return Double4{ hsv.r * 180, hsv.g * 255, hsv.b * 255, hsv.a };
		};

		auto getColor = [&](int pixelIdx) -> Double4 {
			auto pixelStart = pixelIdx * picChannel;
			if (picChannel == 3) {
				return {
					double(data[pixelStart]) / 255.f,
					double(data[pixelStart + 1]) / 255.f,
					double(data[pixelStart + 2]) / 255.f,
					1
				};
			}
			else if (picChannel == 4) {
				return {
				double(data[pixelStart]) / 255.f,
				double(data[pixelStart + 1]) / 255.f,
				double(data[pixelStart + 2]) / 255.f,
				double(data[pixelStart + 3]) / 255.f
				};
			}
			else {
				printf("Unsupported picture channel count!");
				assert(false);
				return -1;
			}
		};
		// 1. calculate look-at table first
		// we do all traverse in one loop
		// all vectors below is Float2 because we need to record the result of Saturate and Value channel
		int threadCount = std::thread::hardware_concurrency() - 1;
		std::vector<double> sumS(threadCount);
		std::fill(sumS.begin(), sumS.end(), 0.f);
		std::vector<double> sumV(threadCount);
		std::fill(sumV.begin(), sumV.end(), 0.f);
		std::vector<double> maxS(threadCount);
		std::fill(maxS.begin(), maxS.end(), 0.f);
		std::vector<double> maxV(threadCount);
		std::fill(maxV.begin(), maxV.end(), 0.f);
		std::vector<double> minS(threadCount);
		std::fill(minS.begin(), minS.end(), 255.f);
		std::vector<double> minV(threadCount);
		std::fill(minV.begin(), minV.end(), 255.f);

		std::atomic<int> workDone{ 0 };
		int width = std::min(300, picWidth);
		int height = std::min(300, picHeight);
		// note: all thread running this lambda will done in this scope, wo se use ref capture
		// if u need to refactor this, taking care of data race and validation problem
		auto calculateParams = [&](int threadIdx, int threadCnt) -> void {
			// traverse 300 * 300 area
			for (int row = 0; row < height; ++row) {
				for (int col = threadIdx; col < width; col += threadCnt) { // some thread will have more loops than others
					auto pixelIdx = row * width + col;
					auto color = getColor(pixelIdx);
					// todo: ensure whether the value in opencv hsv is consist with ours 
					auto hsv = RGBToHSV(color);
					// to opencv format hsv
					hsv = toOpenCVFormatHSV(hsv);
					// update all vectors
					sumS[threadIdx] += hsv.g;
					sumV[threadIdx] += hsv.b;
					if (maxS[threadIdx] < hsv.g) maxS[threadIdx] = hsv.g; // s
					if (maxV[threadIdx] < hsv.b) maxV[threadIdx] = hsv.b; // v
					if (minS[threadIdx] > hsv.g) minS[threadIdx] = hsv.g; // s
					if (minV[threadIdx] > hsv.b) minV[threadIdx] = hsv.b; // v
				}
			}
			++workDone;
		};
		// dispatch jobs
		for (int i = 0; i < threadCount; ++i) {
			std::thread th(calculateParams, i, threadCount);
			th.detach();
		}
		// waiting for all thread complete jobs
		while (workDone < threadCount) {}
		// calculate mean, maxValue and minValue
		// calculate mean first
		auto meanS = std::accumulate(sumS.begin(), sumS.end(), 0.f) / (width * height);
		auto meanV = std::accumulate(sumV.begin(), sumV.end(), 0.f) / (width * height);
		// calculate variance
		double quadSumS = 0;
		double quadSumV = 0;
		for (int row = 0; row < height; ++row) {
			for (int col = 0; col < width; ++col) { // some thread will have more loops than others
				auto pixelIdx = row * width + col;
				auto color = getColor(pixelIdx);
				auto hsv = RGBToHSV(color);
				hsv = Double4{ hsv.r * 180, hsv.g * 255, hsv.b * 255, hsv.a };
				quadSumS += (hsv.g - meanS) * (hsv.g - meanS); // saturate channel
				quadSumV += (hsv.b - meanV) * (hsv.b - meanV); // value channel
			}
		}
		auto varianceS = std::sqrt(quadSumS / (width * height));
		auto varianceV = std::sqrt(quadSumV / (width * height));
		// calculate min/max pixel value
		double maxValueS = *std::max_element(maxS.begin(), maxS.end());
		double maxValueV = *std::max_element(maxV.begin(), maxV.end());
		double minValueS = *std::min_element(minS.begin(), minS.end());
		double minValueV = *std::min_element(minV.begin(), minV.end());
		maxValueS = std::min(maxValueS - meanS, 2 * varianceS);
		maxValueV = std::min(maxValueV - meanV, 2 * varianceV);
		minValueS = std::max(minValueS - meanS, -2 * varianceS);
		minValueV = std::max(minValueV - meanV, -2 * varianceV);

		maxValueS = clamp(maxValueS, -20.f, 20.f);
		maxValueV = clamp(maxValueV, -20.f, 20.f);
		minValueS = clamp(minValueS, -20.f, 20.f);
		minValueV = clamp(minValueV, -20.f, 20.f);
		// calculate channel lower/upper limit

		double bottomS = abs(minValueS);
		double bottomV = abs(minValueV);
		double topS = 255.f - maxValueS;
		double topV = 255.f - maxValueV;

		// refine the input color
		double kS = (topS - bottomS) / 255.f;
		double kV = (topV - bottomV) / 255.f;

		Result result;
		result.bottomS = bottomS;
		result.bottomV = bottomV;
		result.meanS = meanS;
		result.meanV = meanV;
		result.kS = kS;
		result.kV = kV;
		result.version = 1;

		return result;
		//auto diffuseColor = RGBToHSV(mDiffuseColor);
		//diffuseColor = toOpenCVFormatHSV(diffuseColor);
		//bool saturateZero = false;
		//if (diffuseColor.g == 0) saturateZero = true;
		//diffuseColor.g = diffuseColor.g * kS + bottomS;
		//diffuseColor.b = diffuseColor.b * kV + bottomV;
		//// build look-at table
		//unsigned char tableS[256];
		//unsigned char tableV[256];
		//// build tableS
		//if (saturateZero) {
		//	std::fill(tableS, tableS + 255, 0);
		//}
		//else {
		//	for (int i = 0; i <= 255; ++i) {
		//		unsigned char value = (unsigned char)(clamp(diffuseColor.g + i - meanS, 0.f, 255.f));
		//		tableS[i] = value;
		//	}
		//}
		//// build tableV
		//for (int i = 0; i <= 255; ++i) {
		//	unsigned char value = (unsigned char)(clamp(diffuseColor.b + i - meanV, 0.f, 255.f));
		//	tableV[i] = value;
		//}

		//workDone = 0;

		//// change color based on look-at table
		//auto changeColorWithLUT = [&](const Float4& hsv) -> Float4 {
		//	auto cvt = toOpenCVFormatHSV(hsv);
		//	auto res = Float4{
		//		diffuseColor.r / 180.f,
		//		saturateZero ? 0 : double(tableS[(unsigned char)(cvt.g)]) / 255.f,
		//		double(tableV[(unsigned char)(cvt.b)]) / 255.f,
		//		hsv.a
		//	};
		//	return res;
		//};


		//char* output = new char[pixelCount * 4];

		//auto threadFunc = [&](int threadIdx, int threadCnt) {
		//	for (int i = threadIdx; i < pixelCount; i += threadCnt) {
		//		auto pixelColor = getColor(i);

		//		std::swap(pixelColor.r, pixelColor.b);// rgb -> bgr
		//		pixelColor = RGBToHSV(pixelColor);
		//		pixelColor = changeColorWithLUT(pixelColor);
		//		pixelColor = HSVToRGB(pixelColor);

		//		output[i * 4] = char(pixelColor.r * 255);
		//		output[i * 4 + 1] = char(pixelColor.g * 255);
		//		output[i * 4 + 2] = char(pixelColor.b * 255);
		//		output[i * 4 + 3] = char(pixelColor.a * 255);

		//	}
		//	++workDone;
		//};

		//for (int i = 0; i < threadCount; ++i) {
		//	auto thread = std::thread(threadFunc, i, threadCount);
		//	thread.detach();
		//}
		//while (workDone < threadCount) {}


		//printf("fileName = %s\n", savePath.c_str());
		//auto res = stbi_write_png(savePath.c_str(), picWidth, picHeight, 4, output, 0);
		//if (res == 0) {
		//	printf("write picture failure!");
		//	return -1;
		//}
		//delete[] output;
	}


    
    using namespace cv;
    void colorAlterationsSingle(const std::string src, double& ks, double& bottoms, double& means, double& kv, double& bottomv, double& meanv)
    {
        Mat srcimage = imread(src, IMREAD_COLOR);
        if (srcimage.channels() != 3)
            return;
        
        cv::cvtColor(srcimage, srcimage, cv::COLOR_BGR2GRAY);
        cv::cvtColor(srcimage, srcimage, cv::COLOR_GRAY2BGR);
        
        //convert to hsv
        /*Mat bgrColor = Mat(1, 1, CV_8UC3, color);*/
        Mat hsvImage;
        cvtColor(srcimage, hsvImage, COLOR_BGR2HSV);
        
        std::vector<Mat> singleimgs;
        split(hsvImage, singleimgs);
        
        std::vector<std::vector<int>> tables;
        
        int width, height;
        int npixels = 0;
        width = std::min(300, srcimage.cols);
        height = std::min(300, srcimage.rows);
        
        for (int i = 1; i < 3; i++) //HSV
        {
            std::vector<int> table(256, 0);
            Mat curimg;
            resize(singleimgs[i], curimg, Size(width, height), 0, 0, INTER_NEAREST);
            curimg.convertTo(curimg, CV_32FC1);
            npixels = curimg.cols * curimg.rows;
            
            //mean
            cv::Scalar sclmean = sum(curimg) / npixels;
            double mean = sclmean[0];
            
            //variance
            cv::Mat minalsMat = curimg - mean;
            minalsMat = minalsMat.mul(minalsMat);
            cv::Scalar quadratic_sum = sum(minalsMat);
            double variance = sqrt(quadratic_sum[0] / npixels);
            
            //get extremum
            double max1 = 2 * variance;
            double min1 = -2 * variance;
            double max2, min2;
            cv::minMaxLoc(curimg, &min2, &max2, 0, 0);
            
            //reduce effect from noise
            double maxvalue = std::min(max1, max2 - mean);
            double minvalue = std::max(min1, min2 - mean);
            if (minvalue < -20)
                minvalue = -20;
            if (maxvalue > 20)
                maxvalue = 20;
            
            double bottom = abs(minvalue);
            double top = 255 - maxvalue;
            
            //refine the input color
            double k = (top - bottom) / 255;
            
            if (i == 1)
            {
                ks = k;
                bottoms = bottom;
                means = mean;
                
            }
            else if (i == 2)
            {
                kv = k;
                bottomv = bottom;
                meanv = mean;
            }
            
        }
    }
    
    /*
     Result wrap(const std::string& loadPath) {
         int width, height, channel;
         auto* data = stbi_load(loadPath.c_str(), &width, &height, &channel, 0);
         return changeColor(data, width, height, channel);
     }
     */
    
    Result wrap(const std::string& loadPath) {
        int width, height, channel;
        auto* data = stbi_load(loadPath.c_str(), &width, &height, &channel, 0);
        Result result;
        std::string str((const char*)data);
        colorAlterationsSingle(str, result.kS, result.bottomS, result.meanS, result.kV, result.bottomV, result.meanV);
        return result;
    }
    
    Result changeColor_new(const std::string path)
    {
        Result result;
        colorAlterationsSingle(path, result.kS, result.bottomS, result.meanS, result.kV, result.bottomV, result.meanV);
		result.version = 2;
        return result;
    }

}
