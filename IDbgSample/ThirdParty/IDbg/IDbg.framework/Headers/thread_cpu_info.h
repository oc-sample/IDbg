//
//  ThreadInfo.h
//  AppleMiniDumpApp
//
//  Created by mjzheng on 2019/6/28.
//  Copyright © 2019年 mjzheng. All rights reserved.
//

#ifndef THREAD_INFO_H
#define THREAD_INFO_H

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
    FrameList frames;
};

enum class ThreadOptions : uint32_t {
    kBasic = 1 << 0, // 0000 0001
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

int GetThreadInfoById(ThreadStackArray& ls, const ThreadIdArray& id_ls,
                      const ThreadOptions options = ThreadOptions::kBasic);

int GetAllThreadInfo(ThreadStackArray& ls,
                     const ThreadOptions options = ThreadOptions::kBasic);

int GetCpuCore();

} // namesapce IDbg

#endif
