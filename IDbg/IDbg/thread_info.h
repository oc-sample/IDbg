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
#include <map>
#include <mach/mach.h>

typedef std::vector<std::string> FrameList;

struct ThreadStack {
    thread_t th;
    float cpu;
    std::string thread_name;
    FrameList frames;
};

enum ThreadOptions : uint32_t {
    kBasic = 1 << 0, // 0000 0001
    kFrames = 1 << 1,
};

typedef std::vector<ThreadStack> ThreadStackList;
typedef std::map<thread_t, thread_t> IdToIdMap;
typedef std::vector<thread_t> ThreadIdList;

float GetSysCpu();

float GetAppCpu();

int GetThreadInfo(const ThreadOptions options, const ThreadIdList& id_ls, ThreadStackList& ls);

int GetAllThreadInfo(const ThreadOptions options, ThreadStackList& ls);

int GetCpuCore();

#endif

