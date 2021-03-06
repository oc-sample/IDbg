//
//  mini_dump_file.cpp
//  IDbg
//
//  Created by mjzheng on 2021/4/13.
//  Copyright © 2021 mjzheng. All rights reserved.
//
#include "mini_dump_file.h"
#include <unistd.h>

#include <sstream>
#include <vector>

#include "sys_util.h"
#include "ks_dynamic_linker.h"
#include "thread_cpu_info.h"

#if defined(__LP64__)
#define TRACE_FMT         "%-4d%-31s 0x%016lx %s + %lu\n"
#define POINTER_FMT       "0x%016lx"
#define POINTER_SHORT_FMT "0x%lx"
#else
#define TRACE_FMT         "%-4d%-31s 0x%08lx %s + %lu\n"
#define POINTER_FMT       "0x%08lx"
#define POINTER_SHORT_FMT "0x%lx"
#endif

namespace IDbg {

std::string FormatThreadStatck(const ThreadStackArray& ls) {
  std::stringstream ss;
  for (auto i=0; i < ls.size(); ++i) {
    auto thread = ls[i];
    ss << "\nThread " << i << " Name :[" << thread.name << "] Cpu:["
       << thread.cpu << "]\n"
       << "Thread " << thread.th << "\n";
    char buffer[1024];
    for (auto& frame : thread.frames) {
      snprintf(buffer, sizeof(buffer), TRACE_FMT, frame.index,
               frame.module_name.c_str(), frame.address,
               frame.func_name.c_str(), frame.offset);
      ss << std::string(buffer);
    }
  }
  return ss.str();
}

std::string GetAppleFmtHeader() {
  std::stringstream ss;
  ss << "Incident Identifier: " << StringWithUUID() << "\n"
     << "CrashReporter Key: " << GetIDFA() << "\n"
     << "Hardware Model: " << GetDevicePlatform() << "\n"
     << "Process: " << GetProcessName() << "[" << GetProcessId() << "]" << "\n"
     << "Path: " << GetFullPath() << "\n"
     << "Identifier: " << GetIndentifier() << "\n"
     << "Version: " << GetAppVersion() << "\n"
     << "Code Type: " << GetCPUArch() << "\n"
     << "Parent Process: " << getppid() << "\n"
     << "\n"
     << "Date/Time: " << GetDate() << "\n"
     << "OS Version: " << GetSystemName() << " "
     << GetSystemVersion() << " (" << GetOSVersion() << ")\n"
     << "Report Version: 104" << "\n\n";
  return ss.str();
}

typedef std::vector<KSBinaryImage> BinaryImageArray;
void GetBinaryImages(BinaryImageArray* ls) {
  int count = ksdl_imageCount();
  for (int index=0; index < count; index++) {
    KSBinaryImage image = {0};
    if (!ksdl_getBinaryImage(index, &image)) {
        continue;
    }
    ls->push_back(image);
  }
}

std::string GetAppleFmtBinaryImages() {
  std::stringstream ss;
  ss << "\nBinary Images:\n";
  BinaryImageArray ls;
  GetBinaryImages(&ls);
  char buf[1024];
  for (auto& image : ls) {
    const char* last_file = strrchr(image.name, '/');
    const char* name = (last_file == NULL ? image.name : last_file + 1);
    std::string arch_name = GetCPUType(image.cpuType, image.cpuSubType);
    snprintf(buf, sizeof(buf), "0x%llx - 0x%llx %s %s <%s> %s\n", image.address,
             image.address+image.size-1, name, arch_name.c_str(),
             UuidToSting(image.uuid).c_str(), image.name);
    ss << std::string(buf);
  }
  return ss.str();
}

std::string GenerateMiniDump(DumpOptions options) {
  // header
  std::stringstream ss;

  if (options & DumpOptions::kHeader) {
    std::string header = GetAppleFmtHeader();
    ss << header;
  }

  if (options & DumpOptions::kStack) {
    ThreadStackArray ls;
    GetAllThreadInfo(ThreadOptions::kFrames, &ls);
    std::string&& stack = FormatThreadStatck(ls);
    ss << stack;
  }

  // binary images
  if (options & DumpOptions::kImage) {
    std::string image = GetAppleFmtBinaryImages();
    ss << image;
  }

  if (options & DumpOptions::kStack) {
    std::string cpu = "application cpu";
    // [pData appendFormat:@"\napplication cpu{ %.2f}\n", totalCpu];
  }

  return ss.str();
}

}  // namespace IDbg
