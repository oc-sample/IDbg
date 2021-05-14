//
//  ThreadInfo.h
//  AppleMiniDumpApp
//
//  Created by mjzheng on 2019/6/28.
//  Copyright © 2020年 mjzheng. All rights reserved.
//

#ifndef IDBG_THREAD_CPU_INFO_H_
#define IDBG_THREAD_CPU_INFO_H_

#include <vector>
#include <string>
#include <mach/mach.h>

namespace IDbg {

struct FrameInfo {
  int index;
  std::string module_name;
  uintptr_t module_base;
  uintptr_t address;
  uintptr_t offset;
  std::string func_name;
};

typedef std::vector<FrameInfo> FrameList;

struct ThreadStack {
  thread_t th;
  float cpu;
  std::string name;
  std::string module;
  FrameList frames;
};

enum class ThreadOptions : uint32_t {
  kBasic = 1 << 0,  // 0000 0001
  kFrames = 1 << 1,
};

constexpr ThreadOptions operator|(ThreadOptions a, ThreadOptions b) {
  return static_cast<ThreadOptions>(static_cast<int>(a) | static_cast<int>(b));
}

constexpr ThreadOptions operator&(ThreadOptions a, ThreadOptions b) {
  return static_cast<ThreadOptions>(static_cast<int>(a) & static_cast<int>(b));
}

typedef std::vector<ThreadStack> ThreadStackArray;
typedef std::vector<thread_t> ThreadIdArray;

float GetSysCpu();

float GetAppCpu();

int GetThreadInfoById(const ThreadIdArray& id_ls, const ThreadOptions options, ThreadStackArray* ls);

int GetAllThreadInfo(const ThreadOptions options, ThreadStackArray* ls);

int GetCpuCore();

void GetThreadModuleName(ThreadStack& thread, const char** business_module);
}  // namespace IDbg

#endif  // IDBG_THREAD_CPU_INFO_H_

