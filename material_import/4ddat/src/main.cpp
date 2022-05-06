//
//  main.cpp
//  4ddat2admfTest
//
//  Created by yushihang on 2021/3/26.
//

#include "4ddat_to_admf.hpp"
#include "xtex_to_admf.hpp"
#include "exportadmf.h"
#include <stdio.h>
#include <string.h>

#include <string>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <algorithm>

bool iequals(const std::string& a, const std::string& b)
{
    
#if (defined _WIN32) 
    return _stricmp(a.c_str(), b.c_str()) == 0;
#elif (defined __APPLE__)
    return std::equal(a.begin(), a.end(),
                      b.begin(), b.end(),
                      [](char a, char b) {
        return tolower(a) == tolower(b);
    });

#else
    return strncasecmp(a.c_str(), b.c_str(), a.length()) == 0;
#endif
}

///Users/yushihang/Documents/未命名文件夹/temp/1/4ddat2admfTest 1 '/Users/yushihang/Downloads/L210602001(1).4Ddat' /Users/yushihang/Documents/未命名文件夹/temp/1/1.admf /Users/yushihang/Documents/未命名文件夹/temp/1/out 9 9
int main(int argc, const char * argv[]) {
    if (argc < 7)
    {
        const char* exeName = "admf_handler";
        printf("usage: %s <input file type> <input file path> <output admf path> <output dir> <thread count> <png compress level(0-9)>\n\n", exeName);
        printf("input file type:\n");
        printf("    0 : admf (output admf path will be ignored)\n");
        printf("    1 : 4ddat\n");
        printf("    2 : xTex\n\n");
        printf("examples:\n");
        printf("    admf : %s 0 test.admf \"\" /temp/output 2 6 \n", exeName);
        printf("    4ddat : %s 1 test.4ddat output.admf /temp/output 2 6 \n", exeName);
        printf("    xTex : %s 2 test.zip output.admf /temp/output 2 6 \n", exeName);
        return 0;
    }
    
    
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    int type = -1;
    std::string fileType = argv[1];
    if (iequals(fileType, "admf"))
        type = 0;
    else if (iequals(fileType, "4ddat"))
        type = 1;
    else if (iequals(fileType, "xtex"))
        type = 2;
    
    if (type < 0){
        type = std::stoi(argv[1]);
    }
    
    const char* inputPath = argv[2];
    const char* outputAdmf = argv[3];
    const char* extractAdmfDir = argv[4];
    int threadCount = std::stoi(argv[5]);
    int pngComressLevel = std::stoi(argv[6]);
    
    
    if (threadCount < 1)
        threadCount = 1;
    
    if (pngComressLevel < 0)
        pngComressLevel = 0;
    
    if (pngComressLevel > 9)
        pngComressLevel = 9;
    
    printf("type = %d\n", type);
    printf("inputPath = %s\n", inputPath);
    printf("outputAdmf = %s\n", outputAdmf);
    printf("extractAdmfDir = %s\n", extractAdmfDir);
    
    
    switch (type) {
    case 0:
        extractAdmf(inputPath, extractAdmfDir);
        break;
        
    case 1:
        {
           
            _4ddatToAdmf(inputPath, outputAdmf, threadCount, pngComressLevel);
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time_span = t2-t1;
            printf("_4ddatToAdmf:%f\n", time_span.count());
            extractAdmf(outputAdmf, extractAdmfDir);
            std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();

            time_span = t3-t2;
            printf("extractAdmf:%f\n",time_span.count());
            time_span = t3-t1;
            printf("total:%f\n",time_span.count());
        }
        break;
        
    case 2:
        {
            _xtexToAdmf(inputPath, outputAdmf, threadCount, pngComressLevel);
            extractAdmf(outputAdmf, extractAdmfDir);
        }
        break;
        
        
    default:
        break;
    }

	return 0;
}
