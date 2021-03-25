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

struct ThreadStack
{
  thread_t th;
  float cpu;
  std::string threadName;
  FrameList frames;
};

enum THOptions : uint32_t {
  THOptions_basic = 1 << 0, // 0000 0001
  THOptions_frames = 1 << 1,
};

typedef std::vector<ThreadStack> ThreadStackList;
typedef std::map<thread_t, thread_t> IdToIdMap;

float getAppCpu();

float getSysCpu();

int getThreadInfo(ThreadStackList& ls, THOptions options);

int getThreadStackListByID(const IdToIdMap& idMap, ThreadStackList& ls);

int getCpuCore();

#endif

