//
// Created by 郑俊明 on 2021/3/20.
//  Copyright © 2021年 mjzheng. All rights reserved.
//

#ifndef C11_SAMPLE_BASEDEF_H
#define C11_SAMPLE_BASEDEF_H

#include "build_config.h"

#if defined(ANDROID)
#include <android/log.h>
#define LOG(...) { \
   __android_log_print(ANDROID_LOG_DEBUG, "jni", __VA_ARGS__);\
}

#else
#include <stdio.h>
#define LOG(...) { \
   printf("%s %s %s:%d %s ", __DATE__, __TIME__,  __FILE__, __LINE__, __func__); \
   printf(__VA_ARGS__); \
   printf("\n"); \
}

#endif

#if defined(OS_WIN)
  #ifdef IS_BUILDING_SHARED
    #ifdef IDBG_EXPORTS
    #define IDBG_API __declspec(dllexport)
    #else
    #define IDBG_API __declspec(dllimport)
    #endif
  #else
    #define IDBG_API
  #endif
#else
  #define IDBG_API 
#endif

#endif //C11_SAMPLE_BASEDEF_H
