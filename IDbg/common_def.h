//
// Created by 郑俊明 on 2021/3/20.
//

#ifndef C11_SAMPLE_BASEDEF_H
#define C11_SAMPLE_BASEDEF_H

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

#endif //C11_SAMPLE_BASEDEF_H
