//
//  build_config.h
//  IDbg
//
//  Created by mjzheng on 2021/4/25.
//

#ifndef IDBG_IDBG_BUILD_CONFIG_H_
#define IDBG_IDBG_BUILD_CONFIG_H_

#if defined(ANDROID)
#define OS_POSIX
#define OS_ANDROID
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#define OS_POSIX
#if defined(TARGET_OS_OSX) && TARGET_OS_OSX
#define OS_OSX
#endif
#if defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR
#define OS_IOS
#define OS_IOS_SIMULATOR
#elif defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define OS_IOS
#endif
#elif defined(__linux__)
#include <unistd.h>
#define OS_POSIX
#define OS_LINUX
#if defined(__GLIBC__) && !defined(__UCLIBC__)
#define LIBC_GLIBC
#endif
#elif defined (_WINDOWS) || defined (WIN32) || defined (_WIN32) || defined (__WIN32__)
#define OS_WIN
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#pragma warning(disable: 4201)
#endif
#else
#error Please add support for your platform in build_config.h
#endif

#endif  // IDBG_IDBG_BUILD_CONFIG_H_
