//
//  sys_util.h
//  IDbg
//
//  Created by mjzheng on 2021/4/13.
//  Copyright © 2021 mjzheng. All rights reserved.
//

#ifndef sys_util_h
#define sys_util_h

#include <string>
#import <mach/machine.h>

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

#endif /* sys_util_h */
