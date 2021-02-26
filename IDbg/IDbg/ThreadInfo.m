//
//  ThreadInfo.m
//  AppleMiniDumpApp
//
//  Created by mjzheng on 2019/6/28.
//  Copyright © 2019年 mjzheng. All rights reserved.
//

#import "ThreadInfo.h"
#import <mach/mach.h>
#import "KSDynamicLinker.h"
#import <pthread/pthread.h>

#pragma -mark DEFINE MACRO FOR DIFFERENT CPU ARCHITECTURE
#if defined(__arm64__)
#define DETAG_INSTRUCTION_ADDRESS(A) ((A) & ~(3UL))
#define THREAD_STATE_COUNT ARM_THREAD_STATE64_COUNT
#define THREAD_STATE ARM_THREAD_STATE64
#define FRAME_POINTER __fp
#define STACK_POINTER __sp
#define INSTRUCTION_ADDRESS __pc

#elif defined(__arm__)
#define DETAG_INSTRUCTION_ADDRESS(A) ((A) & ~(1UL))
#define THREAD_STATE_COUNT ARM_THREAD_STATE_COUNT
#define THREAD_STATE ARM_THREAD_STATE
#define FRAME_POINTER __r[7]
#define STACK_POINTER __sp
#define INSTRUCTION_ADDRESS __pc

#elif defined(__x86_64__)
#define DETAG_INSTRUCTION_ADDRESS(A) (A)
#define THREAD_STATE_COUNT x86_THREAD_STATE64_COUNT
#define THREAD_STATE x86_THREAD_STATE64
#define FRAME_POINTER __rbp
#define STACK_POINTER __rsp
#define INSTRUCTION_ADDRESS __rip

#elif defined(__i386__)
#define DETAG_INSTRUCTION_ADDRESS(A) (A)
#define THREAD_STATE_COUNT x86_THREAD_STATE32_COUNT
#define THREAD_STATE x86_THREAD_STATE32
#define FRAME_POINTER __ebp
#define STACK_POINTER __esp
#define INSTRUCTION_ADDRESS __eip

#endif

#define CALL_INSTRUCTION_FROM_RETURN_ADDRESS(A) (DETAG_INSTRUCTION_ADDRESS((A)) - 1)

#if defined(__LP64__)
#define TRACE_FMT         @"%-4d%-31s 0x%016lx %s + %lu\n"
#define POINTER_FMT       "0x%016lx"
#define POINTER_SHORT_FMT "0x%lx"
#else
#define TRACE_FMT         @"%-4d%-31s 0x%08lx %s + %lu\n"
#define POINTER_FMT       "0x%08lx"
#define POINTER_SHORT_FMT "0x%lx"
#endif

#define MAX_BACKTRACE 50

typedef struct StackFrameEntry
{
    const struct StackFrameEntry *const previous; // 低地址
    const uintptr_t return_address; // 高地址
} StackFrameEntry;

@implementation ThreadInfo

NSArray* sortThread(NSArray* ls) {
    NSArray *ls1 = [ls sortedArrayUsingComparator:^NSComparisonResult(id obj1, id obj2) {
        ThreadInfo* info1 = (ThreadInfo*)obj1;
        ThreadInfo* info2 = (ThreadInfo*)obj2;
        
        float value1 = [info1.cpu floatValue];
        float value2 = [info2.cpu floatValue];
        if (value1 < value2) {
           return NSOrderedDescending;
        }else if (value1 == value2){
           return NSOrderedSame;
        }else{
           return NSOrderedAscending;
        }
        return NSOrderedSame;
    }];
    return ls1;
}

NSString* getAllThreadStack(float* appCpu)
{
    thread_act_array_t threads;
    mach_msg_type_number_t thread_count = 0;
    const task_t this_task = mach_task_self();
    kern_return_t kr = task_threads(this_task, &threads, &thread_count);
    if(kr != KERN_SUCCESS)
    {
        return @"Fail to enum threads";
    }
    
    // 功能
    *appCpu = 0;
    NSMutableArray* ls = [[NSMutableArray alloc] init];
    for (int i=0; i<thread_count; i++)
    {
        thread_t thread = threads[i];
        NSString* threadName = nil;
        float threadCpu = getThreadCpuEx(thread, &threadName);
        ThreadInfo* info = [[ThreadInfo alloc] init];
        info.cpu = [NSNumber numberWithFloat:threadCpu];
        info.name = threadName;
        info.th = [NSNumber numberWithLong:(long)thread];
      
        NSString* stack = getThreadStack(i, thread);
        info.stack = stack;
        [ls addObject:info];
        *appCpu += threadCpu;
    }
    
    // 排序
    NSArray *ls1 = sortThread(ls);
    
    // 后处理
    NSMutableString *resultString = [[NSMutableString alloc] init];
    for (int i=0; i< ls1.count; i++)
    {
        ThreadInfo* info = (ThreadInfo*)ls1[i];
        [resultString appendFormat:@"\nThread %d Name :[%@] Cpu:[%.2f]\n", i, info.name, [info.cpu floatValue]];
        [resultString appendFormat:@"Thread %d:\n", i];
        [resultString appendFormat:@"%@", info.stack];
    }
    
    NSString* allThreadStack = [resultString copy];
    NSLog(@"%@", allThreadStack);
    NSLog(@"app cpu %.2f", *appCpu);
    return allThreadStack;
}

// C function
NSString* getThreadStack(int index, thread_t thread)
{
    // 获取线程的ebp 和esp, 用于调用链重构
    _STRUCT_MCONTEXT machineContext;
    if(!thread_get_state_ex(thread, &machineContext))
    {
        return [NSString stringWithFormat:@"Fail to get thread[%u] state", thread];
    }
    const uintptr_t instructionAddress = mach_instructionAddress(&machineContext);
    if(instructionAddress == 0)
    {
        [NSString stringWithFormat:@"Fail to get thread[%u] instruction address", thread];
    }
   
    int i = 0;
    uintptr_t backtraceBuffer[MAX_BACKTRACE];
    memset(backtraceBuffer, 0, sizeof(backtraceBuffer));
    backtraceBuffer[i++] = instructionAddress;
    uintptr_t linkRegister = mach_linkRegister(&machineContext);
    if (linkRegister)
    {
        backtraceBuffer[i++] = linkRegister;
    }
    
    StackFrameEntry frame = {0};
    const uintptr_t framePtr = mach_framePointer(&machineContext);

    if(framePtr == 0 || mach_copyMem((void *)framePtr, &frame, sizeof(frame)) != KERN_SUCCESS)
    {
        NSLog(@"fail to get frame pointer");
        return @"Fail to get frame pointer";
    }

    for(; i < MAX_BACKTRACE; i++)
    {
        backtraceBuffer[i] = frame.return_address;
        if(backtraceBuffer[i] == 0 || frame.previous == 0
           || mach_copyMem(frame.previous, &frame,sizeof(frame)) != KERN_SUCCESS)
        {
            break;
        }
    }
    
    NSMutableString *resultString = [[NSMutableString alloc] init];
    for (int j=0; j<i; j++)
    {
        NSString* frameEntry = getFrameEntry(j, backtraceBuffer[j]);
        [resultString appendString: frameEntry];
    }
    
    [resultString appendFormat:@"\n"];
    
    return [resultString copy];
}

#pragma -mark HandleMachineContext
bool thread_get_state_ex(thread_t thread, _STRUCT_MCONTEXT *machineContext)
{
    mach_msg_type_number_t state_count = THREAD_STATE_COUNT;
    kern_return_t kr = thread_get_state(thread, THREAD_STATE, (thread_state_t)&machineContext->__ss, &state_count);
    return (kr == KERN_SUCCESS);
}

uintptr_t mach_instructionAddress(mcontext_t const machineContext)
{
    return machineContext->__ss.INSTRUCTION_ADDRESS;
}

uintptr_t mach_linkRegister(mcontext_t const machineContext)
{
#if defined(__i386__) || defined(__x86_64__)
    return 0;
#else
    return machineContext->__ss.__lr;
#endif
}

uintptr_t mach_framePointer(mcontext_t const machineContext)
{
    return machineContext->__ss.FRAME_POINTER;
}

uintptr_t mach_stackPointer(mcontext_t const machineContext)
{
    return machineContext->__ss.STACK_POINTER;
}

kern_return_t mach_copyMem(const void *const src, void *const dst, const size_t numBytes)
{
    vm_size_t bytesCopied = 0;
    return vm_read_overwrite(mach_task_self(), (vm_address_t)src, (vm_size_t)numBytes, (vm_address_t)dst, &bytesCopied);
}

NSString* getFrameEntry(const int entryNum, const uintptr_t address)
{
    Dl_info dlInfo;
    ksdl_dladdr(address, &dlInfo);
    
    char faddrBuff[20];
    char saddrBuff[20];
    const char* fname = bs_lastPathEntry(dlInfo.dli_fname);
    if(fname == NULL)
    {
        sprintf(faddrBuff, POINTER_FMT, (uintptr_t)dlInfo.dli_fbase);
        fname = faddrBuff;
    }
    
    uintptr_t offset = address - (uintptr_t)dlInfo.dli_saddr;
    const char* sname = dlInfo.dli_sname;
    const char* pUnused = "<redacted>";
    if(sname == NULL || 0 == strcmp(sname, pUnused))
    {
        sprintf(saddrBuff, POINTER_FMT, (uintptr_t)dlInfo.dli_fbase);
        sname = saddrBuff;
        offset = address - (uintptr_t)dlInfo.dli_fbase;
    }
    
    return [NSString stringWithFormat:TRACE_FMT , entryNum, fname, (uintptr_t)address, sname, offset];
}

float getThreadCpuEx(thread_t thread, NSString** pThreadName)
{
    if (numbers == 0) {
        numbers = [NSProcessInfo processInfo].activeProcessorCount;
        NSLog(@"system cpu core %d", numbers);
    }
    
    
    float cpu = 0;
    mach_msg_type_number_t thread_info_count = THREAD_INFO_MAX;
    thread_info_data_t thinfo;
    kern_return_t kr = thread_info(thread, THREAD_EXTENDED_INFO, (thread_info_t)thinfo, &thread_info_count);
    if (kr != KERN_SUCCESS)
    {
        return cpu;
    }
    
    thread_extended_info_t basic_info_th = (thread_extended_info_t)thinfo;
    if (!(basic_info_th->pth_flags & TH_FLAGS_IDLE))
    {
        cpu = basic_info_th->pth_cpu_usage / (float)TH_USAGE_SCALE * 100.0 / numbers;
    }
    *pThreadName = [NSString stringWithUTF8String:basic_info_th->pth_name];
    return cpu;
}

const char* bs_lastPathEntry(const char* const path)
{
    if(path == NULL)
    {
        return NULL;
    }
    
    char* lastFile = strrchr(path, '/');
    return lastFile == NULL ? path : lastFile + 1;
}


static NSUInteger numbers = 0;

NSArray* getAllThreadBasicInfo(float* appCpu) {
    thread_act_array_t threads;
    mach_msg_type_number_t thread_count = 0;
    const task_t this_task = mach_task_self();
    kern_return_t kr = task_threads(this_task, &threads, &thread_count);
    if(kr != KERN_SUCCESS)
    {
        return nil;
        //return @"Fail to enum threads";
    }
    
    *appCpu = 0;
    NSMutableArray* ls = [[NSMutableArray alloc] init];
    for (int i=0; i<thread_count; i++)
    {
        thread_t thread = threads[i];
        NSString* threadName = nil;
        float threadCpu = getThreadCpuEx(thread, &threadName);
//        const char* tmp = "mj";
//        thread_set_thread_name(thread, tmp);
        //thread_set_thread_name(thread, "mj");
        if (threadCpu > 0 ) {
            ThreadInfo* info = [[ThreadInfo alloc] init];
            info.cpu = [NSNumber numberWithFloat:threadCpu];
            info.th = [NSNumber numberWithLong:(long)thread];
            info.name = threadName;
            [ls addObject:info];
            *appCpu += threadCpu;
        }
    }
    
    // 排序
    NSArray *ls1 = sortThread(ls);
    
    NSMutableArray* ls2 = updateHistory(ls1);
    
    return ls2;
}

NSNumber* getIndex(ThreadInfo* item) {
    static NSMutableDictionary* dic = nil;
    static int count = 0;
    if (dic == nil) {
        dic = [[NSMutableDictionary alloc]init];
    }
    
    if ([dic objectForKey: item.th] == nil) {
        dic[item.th] = [NSNumber numberWithInt:count];
        count++;
    }
    return dic[item.th];
}

BOOL isFound(NSArray* ls, ThreadInfo* item) {
    for (ThreadInfo* info in ls) {
        if (item.th == info.th) {
            item.cpu = info.cpu;
            item.stack = info.stack;
            return TRUE;
        }
    }
    return false;
}

NSArray* updateHistory(NSArray* ls) {
    static NSMutableArray* history = nil;
    if (history == nil) {
        history = [[NSMutableArray alloc] init];
    }
    
    // update history thread
    for (int i=0; i<history.count; i++) {
        ThreadInfo* info = history[i];
        if (isFound(ls, info)) {
            // exist, update
            history[i] = info;
        } else {
            //no exist, reset
            info.cpu = [NSNumber numberWithLong:0];
            info.stack = @"";
            history[i] = info;
        }
    }
    
    // add new thread
    for (int i=0; i<ls.count; i++) {
        ThreadInfo* info = ls[i];
        if (!isFound(history, info)) {
            [history addObject:info];
        }
    }
    return history;
}

typedef struct ana_cpu_load_info {
    
    natural_t cpu_user;
    natural_t cpu_system;
    natural_t cpu_idle;
    natural_t cpu_nice;
}ana_cpu_load_info;

float getSysCpu()
{
    ana_cpu_load_info cpuloadinfo;
    static struct ana_cpu_load_info lastcpuloadinfo = {0, 0, 0, 0};
    mach_port_t host_port;
    mach_msg_type_number_t host_size;
    host_cpu_load_info_data_t host_load;
    host_port = mach_host_self();
    host_size = sizeof(host_cpu_load_info_data_t) / sizeof(integer_t);
    int ret = host_statistics(host_port, HOST_CPU_LOAD_INFO, (host_info_t)&host_load, &host_size);
    if (KERN_SUCCESS != ret) {
        
        return -1;
    }
    else {
        cpuloadinfo.cpu_user = host_load.cpu_ticks[0] - lastcpuloadinfo.cpu_user;
        cpuloadinfo.cpu_system = host_load.cpu_ticks[1] - lastcpuloadinfo.cpu_system;
        cpuloadinfo.cpu_idle = host_load.cpu_ticks[2] - lastcpuloadinfo.cpu_idle;
        cpuloadinfo.cpu_nice = host_load.cpu_ticks[3] - lastcpuloadinfo.cpu_nice;
        memcpy(&lastcpuloadinfo, &host_load, sizeof(lastcpuloadinfo));
    }
    
    return 100.0*((float)(cpuloadinfo.cpu_user + cpuloadinfo.cpu_system)/(float)(cpuloadinfo.cpu_user + cpuloadinfo.cpu_system+cpuloadinfo.cpu_idle + cpuloadinfo.cpu_nice));
    
}

NSString* getAllThreadStr()
{
    float appCpu = 0;
    float sysCpu = getSysCpu();
    NSArray* ls = getAllThreadBasicInfo(&appCpu);
    NSMutableString* header = [[NSMutableString alloc] init];
    [header appendString:@"header\tsys\tapp\t"];
    for (int i=0; i<ls.count; i++) {
        ThreadInfo* info = (ThreadInfo*)ls[i];
        NSString* threadName = @"";
        if (info != nil) {
            if (info.name != nil && ![info.name isEqual:@""]) {
                threadName = info.name;
            } else {
                threadName = [NSString stringWithFormat:@"%lu", [info.th longValue]];
            }
        }
        [header appendFormat:@"%@\t", threadName];
    }
    
    NSMutableString* line = [[NSMutableString alloc] init];
    [line appendFormat:@"line\t%0.2f\t%0.2f\t", sysCpu, appCpu];
    for (int i=0; i<ls.count; i++) {
        ThreadInfo* info = (ThreadInfo*)ls[i];
        [line appendFormat:@"%@\t", info.cpu];
    }
    
    [header appendFormat:@"\n%@\n", line];
    
    saveToFile(header);
    
    return header;
}

void saveToFile(NSString*pData)
{
    static NSFileHandle *fileHandle = nil;
    if (fileHandle == nil) {
        NSString* tmpPath = NSTemporaryDirectory();
        NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
        [formatter setDateFormat:@"yyyMMddHHmmssSSS"];
        NSString* dateTime = [formatter stringFromDate:[NSDate date]];
        NSString* fileName = [NSString stringWithFormat:@"%@manual_stack/%@_cpu.txt", tmpPath, dateTime];
        NSFileManager *fileManager = [NSFileManager defaultManager];
        [fileManager createFileAtPath:fileName contents:nil attributes:nil];
        fileHandle = [NSFileHandle fileHandleForWritingAtPath:fileName];
        NSLog(@"create file handle %@", fileHandle);
    }

    NSData* stringData  = [pData dataUsingEncoding:NSUTF8StringEncoding];
    [fileHandle writeData:stringData];
    NSLog(@"write data %@", stringData);
}

void createFileDirectories()
{
    NSString* tmpPath = NSTemporaryDirectory();
    NSString *picPath = [tmpPath stringByAppendingPathComponent:@"manual_stack"];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL isDir = FALSE;
    BOOL isDirExist = [fileManager fileExistsAtPath:picPath isDirectory:&isDir];
    if(!(isDirExist && isDir))
    {
        BOOL bCreateDir = [fileManager createDirectoryAtPath: picPath withIntermediateDirectories:YES attributes:nil error:nil];
        if(!bCreateDir)
        {
            NSLog(@"fail to create manual_stack direction");
        }
    }
}

@end
