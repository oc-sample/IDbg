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

typedef std::vector<std::string> FrameList;

struct ThreadStack
{
  float cpu;
  std::string threadName;
  FrameList frames;
};

typedef std::vector<ThreadStack> ThreadStackList;

float getAppCpu();

float getSysCpu();

int getThreadStackList(ThreadStackList& ls);

#endif

