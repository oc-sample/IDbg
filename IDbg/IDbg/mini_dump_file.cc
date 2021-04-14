//
//  mini_dump_file.cpp
//  IDbg
//
//  Created by mjzheng on 2021/4/13.
//  Copyright © 2021 mjzheng. All rights reserved.
//
#include "mini_dump_file.h"
#include <sstream>
#include "sys_util.h"
#include "ks_dynamic_linker.h"
#include <unistd.h>


std::string GetAppleFmtHeader();

std::string GetThreadStatck();

std::string GetAppleFmtBinaryImages();

std::string GenerateMiniDump(DumpOptions options) {
    // header
    std::stringstream ss;
    
    if (options & kHeader) {
        std::string header = GetAppleFmtHeader();
    }

    // thread backtrace
    float totalCpu = 0;
    if (options & kStack) {
        std::string stack = GetThreadStatck();
    }

    // binary images
    if (options & kImage) {
        std::string image = GetAppleFmtBinaryImages();
    }
    
    if (options & kStack) {
        std::string cpu = "application cpu";
        //[pData appendFormat:@"\napplication cpu{ %.2f}\n", totalCpu];
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
       << "OS Version:" << GetSystemName() << " " << GetSystemVersion() << " (" << GetOSVersion() << ")\n"
       << "Report Version: 104" << "\n\n";
    return ss.str();
}

std::string GetThreadStatck() {
    return "";
    //return getAllThreadStack(totalCpu);
}

struct BinaryImage {
    uint64_t start_address;
    uint64_t end_address;
    std::string name;
    std::string arch_name;
    std::string uuid;
    std::string full_path;
};

typedef std::vector<BinaryImage> BinaryImageArray;


void GetBinaryImages(BinaryImageArray& ls) {
    
    int count = ksdl_imageCount();
    for (int index=0; index<count; index++) {
        KSBinaryImage image = {0};
        if(!ksdl_getBinaryImage(index, &image)) {
            continue;
        }
        
        BinaryImage im;
        im.start_address = image.address;
        im.end_address = image.address + image.size - 1;
        im.arch_name = GetCPUType(image.cpuType, image.cpuSubType);
        im.full_path = image.name;
        im.name = image.name;
        im.uuid = std::string( (char*)image.uuid );
        ls.push_back(im);
    }
}

std::string GetAppleFmtBinaryImages() {
    std::stringstream ss;
    ss << "\nBinary Images:\n";
    BinaryImageArray ls;
    GetBinaryImages(ls);
    char buf[1024];
    for (auto& image : ls) {
        snprintf(buf, 1024, "0x%llx - 0x%llx %s %s <%s> %s\n", image.start_address, image.end_address,
                 image.name.c_str(), image.arch_name.c_str(), image.uuid.c_str(), image.full_path.c_str());
        ss << std::string(buf);
    }
    return ss.str();
}

//-(void) saveToFile:(NSString*) pData
//{
//    NSString* tmpPath = NSTemporaryDirectory();
//    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
//    [formatter setDateFormat:@"yyyMMddHHmmssSSS"];
//    NSString* dateTime = [formatter stringFromDate:[NSDate date]];
//
//    NSString* fileName = [NSString stringWithFormat:@"%@manual_stack/%@_ori.crash", tmpPath, dateTime];
//
//    [pData writeToFile:fileName atomically:YES encoding:NSUTF8StringEncoding error:nil];
//
//}
