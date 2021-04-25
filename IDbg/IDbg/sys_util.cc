//
//  sys_util.cpp
//  IDbg
//
//  Created by 郑俊明 on 2021/4/24.
//

#include "sys_util.h"

#if defined(OS_WIN)
#include <windows.h>
const DWORD kVCThreadNameException = 0x406D1388;
#pragma pack(push, 8)
typedef struct tagTHREADNAME_INFO {
  DWORD dwType; // Must be 0x1000.
  LPCSTR szName; // Pointer to name (in user addr space).
  DWORD dwThreadID; // Thread ID (-1=caller thread).
  DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)
#else
#include <pthread.h>
#endif

void SetThreadName(const std::string& name) {
  if (name.empty()) {
    return;
  }

#if defined(OS_MACOSX) || defined(OS_IOS)
  pthread_setname_np(name.c_str());
#elif defined(OS_LINUX) || defined(OS_ANDROID)
  pthread_setname_np(pthread_self(), name.c_str());
#elif defined(OS_WIN)
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = name.c_str();
  info.dwThreadID = GetCurrentThreadId();
  info.dwFlags = 0;
  __try {
    RaiseException(kVCThreadNameException, 0, sizeof(info) / sizeof(DWORD),
      reinterpret_cast<DWORD_PTR*>(&info));
  } __except(EXCEPTION_CONTINUE_EXECUTION) {
  }
#endif
}
