#pragma once
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#define DYNAMIC_OUTPUT __declspec(dllexport)
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__GLIBC__)
#define DYNAMIC_OUTPUT __attribute__ ((visibility("default")))
#else
#define DYNAMIC_OUTPUT
#endif
#include <string>

#define _4DSTC_CHANGEC_COLOR_KEY "4dstc.changeColor"

namespace  CHANGE_COLOR {
	struct DYNAMIC_OUTPUT Result
	{
		double bottomS = 0, bottomV = 0, meanS = 0, meanV = 0, kS = 0, kV = 0;
        int version = 1;
	};
	Result DYNAMIC_OUTPUT wrap(const std::string& loadPath);
	Result DYNAMIC_OUTPUT changeColor(unsigned char* data, int picWidth, int picHeight, int picChannel);
    Result DYNAMIC_OUTPUT changeColor_new(const std::string path);
}
