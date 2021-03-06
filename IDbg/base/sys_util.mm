//
//  sys_util.cpp
//  IDbg
//
//  Created by mjzheng on 2021/4/13.
//  Copyright © 2021 mjzheng. All rights reserved.
//

#include "sys_util.h"
#include <sys/sysctl.h>
#include <sstream>
#import <Foundation/Foundation.h>
#import <AdSupport/AdSupport.h>
#include <time.h>
#if TARGET_OS_IOS
#import <UIKit/UIDevice.h>
#endif

namespace IDbg {

std::string StringWithUUID() {
    CFUUIDRef uuidObj = CFUUIDCreate(nil);//create a new UUI
    NSString* uuidString = (__bridge NSString*)CFUUIDCreateString(nil, uuidObj);
    CFRelease(uuidObj);
    std::string uuid = [uuidString UTF8String];
    return uuid;
}

std::string GetIDFA() {
//    NSString *idfa = [[[ASIdentifierManager sharedManager]
    //advertisingIdentifier] UUIDString];
//    NSLog(@"%@", idfa);
//    return [idfa UTF8String];
    return "";
}

std::string GetDevicePlatform() {
    static std::string platform("");
    if (platform == "") {
        size_t size;
        sysctlbyname("hw.machine", NULL, &size, NULL, 0);
        char *machine = (char *)malloc(size);
        sysctlbyname("hw.machine", machine, &size, NULL, 0);
        if(machine == NULL){
            platform = "i386";
        } else {
            platform = machine;
        }
        free(machine);
    }
    return platform;
}

std::string GetOSVersion() {
    size_t size = 0;
    sysctlbyname("kern.osversion", NULL, &size, NULL, 0);
    char* osversion = (char*)malloc(size);
    sysctlbyname("kern.osversion", osversion, &size, NULL, 0);
    return std::string(osversion);
}

std::string GetDate() {
    time_t current_time = time(NULL);
    struct tm *info = localtime(&current_time);
    char buf[64];
    strftime(buf, 64, "%Y-%m-%d %H:%M:%S", info);
    return std::string(buf);
}

std::string GetCPUArch() {
    // ARM-64, ARM, x86-64, or x86.
    cpu_type_t cpu_type = 0;
    size_t size = sizeof(cpu_type);
    sysctlbyname("hw.cputype", &cpu_type, &size, NULL, 0);
    std::string str_cpu_arch = "unknown";
    switch(cpu_type) {
        case CPU_TYPE_ARM:
            str_cpu_arch = "ARM";
            break;
#ifdef CPU_TYPE_ARM64
        case CPU_TYPE_ARM64:
            str_cpu_arch =  "ARM-64";
            break;
#endif
        case CPU_TYPE_X86:
            str_cpu_arch = "X86";
            break;
        case CPU_TYPE_X86_64:
            str_cpu_arch = "X86_64";
            break;
        default:
            break;
    }
    
    return str_cpu_arch;
}

std::string GetCPUType(cpu_type_t majorCode, cpu_subtype_t minorCode){
    switch(majorCode){
        case CPU_TYPE_ARM:{
            switch (minorCode) {
                case CPU_SUBTYPE_ARM_V6:
                    return "armv6";
                case CPU_SUBTYPE_ARM_V7:
                    return "armv7";
                case CPU_SUBTYPE_ARM_V7F:
                    return "armv7f";
                case CPU_SUBTYPE_ARM_V7K:
                    return "armv7k";
#ifdef CPU_SUBTYPE_ARM_V7S
                case CPU_SUBTYPE_ARM_V7S:
                    return "armv7s";
#endif
            }
            return "arm";
        }
#ifdef CPU_TYPE_ARM64
        case CPU_TYPE_ARM64:
            return "arm64";
#endif
        case CPU_TYPE_X86:
            return "i386";
        case CPU_TYPE_X86_64:
            return "x86_64";
    }
    std::stringstream ss;
    ss << "unknown(" << majorCode << "," << minorCode << ")";
    return ss.str();
}

std::string GetAppVersion() {
    NSDictionary* info_dict = [[NSBundle mainBundle] infoDictionary];
    NSString* app_version = [NSString stringWithFormat:@"%@ (%@)",
                             info_dict[@"CFBundleVersion"],
                             info_dict[@"CFBundleShortVersionString"]];
    return [app_version UTF8String];
}

std::string GetIndentifier() {
    NSDictionary* info_dict = [[NSBundle mainBundle] infoDictionary];
    return [ info_dict[@"CFBundleIdentifier"] UTF8String];
}

std::string GetProcessName() {
    return [[NSProcessInfo processInfo].processName UTF8String];
}

int GetProcessId() {
    return [NSProcessInfo processInfo].processIdentifier;
}

std::string GetFullPath() {
    NSDictionary* info_dict = [[NSBundle mainBundle] infoDictionary];
    NSString* executable_name = info_dict[@"CFBundleExecutable"];
    NSString* bundle_path = [[NSBundle mainBundle] bundlePath];
    NSString* full_path = [bundle_path stringByAppendingPathComponent:
                           executable_name];
    return [full_path UTF8String];
}


std::string GetSystemName() {
#if TARGET_OS_IOS
    NSString* sys_name = [[UIDevice currentDevice] systemName];
    return [sys_name UTF8String];
#else
    return "";
#endif
}

std::string GetSystemVersion() {
#if TARGET_OS_IOS
    return [ [[UIDevice currentDevice] systemVersion] UTF8String];
#else
    return "";
#endif
}

std::string UuidToSting(const uint8_t* bytes) {
    CFUUIDRef uuid_ref = CFUUIDCreateFromUUIDBytes(NULL, *((CFUUIDBytes*)bytes));
    if (uuid_ref != NULL) {
        NSString* tmp = (__bridge_transfer NSString*)CFUUIDCreateString(NULL, uuid_ref);
        std::string str_uuid = [tmp UTF8String];
        CFRelease(uuid_ref);
        return str_uuid;
    }
    return "";
}

void SaveToFile(const std::string& data) {
    NSString* pData = [NSString stringWithUTF8String:data.c_str()];
    NSString* tmpPath = NSTemporaryDirectory();
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyMMddHHmmssSSS"];
    NSString* dateTime = [formatter stringFromDate:[NSDate date]];
    NSString* fileName = [NSString stringWithFormat:@"%@manual_stack/%@_ori.crash", tmpPath, dateTime];
    [pData writeToFile:fileName atomically:YES encoding:NSUTF8StringEncoding error:nil];
}

void CreateFileDirectories() {
    NSString* tmpPath = NSTemporaryDirectory();
    NSString *picPath = [tmpPath stringByAppendingPathComponent:@"manual_stack"];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL isDir = FALSE;
    BOOL isDirExist = [fileManager fileExistsAtPath:picPath isDirectory:&isDir];
    if(!(isDirExist && isDir)) {
        BOOL bCreateDir = [fileManager createDirectoryAtPath: picPath withIntermediateDirectories:YES attributes:nil error:nil];
        if(!bCreateDir) {
            NSLog(@"fail to create manual_stack direction");
        }
    }
}

}

