//
//  sys_util.h
//  IDbg
//
//  Created by mjzheng on 2021/4/13.
//  Copyright © 2021 mjzheng. All rights reserved.
//

#ifndef IDBG_IDBG_SYS_UTIL_H_
#define IDBG_IDBG_SYS_UTIL_H_

#include <string>
#include "build_config.h"
#include "common_def.h"

#if defined(OS_IOS) || defined(OS_OSX)
#import <mach/machine.h>
#endif

namespace IDbg {

#if defined(OS_IOS) || defined(OS_OSX)
std::string StringWithUUID();

std::string GetIDFA();

std::string GetDevicePlatform();

std::string GetOSVersion();

std::string GetDate();

std::string GetCPUArch();

std::string GetCPUType(cpu_type_t majorCode, cpu_subtype_t minorCode);

std::string GetSystemName();

std::string GetSystemVersion();

std::string GetOSVersion();

std::string GetAppVersion();

std::string GetIndentifier();

std::string GetProcessName();

std::string GetFullPath();

int GetProcessId();

std::string UuidToSting(const uint8_t* bytes);

#endif
IDBG_API void SetThreadName(const std::string& name);
}

#endif  // IDBG_IDBG_SYS_UTIL_H_
