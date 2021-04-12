//
//  ThreadInfo.m
//  AppleMiniDumpApp
//
//  Created by mjzheng on 2019/6/28.
//  Copyright © 2019年 mjzheng. All rights reserved.
//

#include "thread_info.h"
#include <pthread/pthread.h>
#include "KSDynamicLinker.h"
#include <sys/sysctl.h>

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

//#if defined(__LP64__)
//#define TRACE_FMT         "%-4d%-31s 0x%016lx %s + %lu"
//#define POINTER_FMT       "0x%016lx"
//#define POINTER_SHORT_FMT "0x%lx"
//#else
//#define TRACE_FMT         "%-4d%-31s 0x%08lx %s + %lu"
//#define POINTER_FMT       "0x%08lx"
//#define POINTER_SHORT_FMT "0x%lx"
//#endif

#if defined(__LP64__)
#define TRACE_FMT         "%d %s 0x%016lx %s + %lu"
#define POINTER_FMT       "0x%016lx"
#define POINTER_SHORT_FMT "0x%lx"
#else
#define TRACE_FMT         "%d %s 0x%08lx %s + %lu"
#define POINTER_FMT       "0x%08lx"
#define POINTER_SHORT_FMT "0x%lx"
#endif

#define MAX_BACKTRACE 50


std::string GetFrameEntry(const int entryNum, const uintptr_t address);

int GetThreadCpuAndName(thread_t thread, float& cpu, std::string& threadName);

typedef struct ana_cpu_load_info {
    natural_t cpu_user;
    natural_t cpu_system;
    natural_t cpu_idle;
    natural_t cpu_nice;
}ana_cpu_load_info;

float GetSysCpu() {
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

float GetAppCpu() {
    thread_array_t         thread_list;
    mach_msg_type_number_t thread_count;
    kern_return_t          kr;
    kr = task_threads(mach_task_self(), &thread_list, &thread_count);
    if (kr != KERN_SUCCESS) {
        return -1;
    }

    float tot_cpu = 0;
    std::string thread_name;
    for (int j = 0; j < thread_count; j++) {
        float thread_cpu = 0;
        int ret = GetThreadCpuAndName(thread_list[j], thread_cpu, thread_name);
        if (ret != 0) {
            kr = vm_deallocate(mach_task_self(), (vm_offset_t)thread_list, thread_count * sizeof(thread_t));
            assert(kr == KERN_SUCCESS);
            return -1;
        }
        tot_cpu += thread_cpu;
    } // for each thread
    kr = vm_deallocate(mach_task_self(), (vm_offset_t)thread_list, thread_count * sizeof(thread_t));
    assert(kr == KERN_SUCCESS);
    return tot_cpu;
}

typedef struct StackFrameEntry {
    const struct StackFrameEntry *const previous; // 低地址
    const uintptr_t return_address; // 高地址
} StackFrameEntry;

#pragma -mark HandleMachineContext
bool thread_get_state_ex(thread_t thread, _STRUCT_MCONTEXT *machineContext) {
    mach_msg_type_number_t state_count = THREAD_STATE_COUNT;
    kern_return_t kr = thread_get_state(thread, THREAD_STATE, (thread_state_t)&machineContext->__ss, &state_count);
    return (kr == KERN_SUCCESS);
}

uintptr_t mach_instructionAddress(mcontext_t const machineContext) {
    return machineContext->__ss.INSTRUCTION_ADDRESS;
}

uintptr_t mach_linkRegister(mcontext_t const machineContext) {
#if defined(__i386__) || defined(__x86_64__)
    return 0;
#else
    return machineContext->__ss.__lr;
#endif
}

uintptr_t mach_framePointer(mcontext_t const machineContext) {
    return machineContext->__ss.FRAME_POINTER;
}

uintptr_t mach_stackPointer(mcontext_t const machineContext) {
    return machineContext->__ss.STACK_POINTER;
}

kern_return_t mach_copyMem(const void *const src, void *const dst, const size_t numBytes) {
    vm_size_t bytesCopied = 0;
    return vm_read_overwrite(mach_task_self(), (vm_address_t)src, (vm_size_t)numBytes, (vm_address_t)dst, &bytesCopied);
}

const char* bs_lastPathEntry(const char* const path) {
    if(path == NULL) {
        return NULL;
    }
    const char* lastFile = strrchr(path, '/');
    return lastFile == NULL ? path : lastFile + 1;
}

// C function
int GetThreadStack(int index, thread_t thread, FrameList& frameList) {
  // 获取线程的ebp 和esp, 用于调用链重构
  _STRUCT_MCONTEXT machineContext;
  if(!thread_get_state_ex(thread, &machineContext)) {
    return -1;
  }
  const uintptr_t instructionAddress = mach_instructionAddress(&machineContext);
  if(instructionAddress == 0) {
    return -1;
  }
 
  int i = 0;
  uintptr_t backtraceBuffer[MAX_BACKTRACE];
  memset(backtraceBuffer, 0, sizeof(backtraceBuffer));
  backtraceBuffer[i++] = instructionAddress;
  uintptr_t linkRegister = mach_linkRegister(&machineContext);
  if (linkRegister) {
      backtraceBuffer[i++] = linkRegister;
  }
  
  StackFrameEntry frame = {0};
  const uintptr_t framePtr = mach_framePointer(&machineContext);

  if(framePtr == 0 || mach_copyMem((void *)framePtr, &frame, sizeof(frame)) != KERN_SUCCESS) {
    return -1;
  }

  for(; i < MAX_BACKTRACE; i++) {
      backtraceBuffer[i] = frame.return_address;
      if(backtraceBuffer[i] == 0 || frame.previous == 0
         || mach_copyMem(frame.previous, &frame,sizeof(frame)) != KERN_SUCCESS) {
          break;
      }
  }
  
  for (int j=0; j<i; j++) {
    std::string frame = GetFrameEntry(j, backtraceBuffer[j]);
    frameList.push_back(frame);
  }
  return 0;
}


int GetThreadInfo(const IdToIdMap& filter_map, const ThreadOptions options, ThreadStackList& ls) {
    thread_act_array_t threads;
    mach_msg_type_number_t thread_count = 0;
    const task_t this_task = mach_task_self();
    kern_return_t kr = task_threads(this_task, &threads, &thread_count);
    if(kr != KERN_SUCCESS) {
        return -1;
    }
  
  // 功能
    for (int i=0; i<thread_count; i++) {
        thread_t thread = threads[i];
        if (!filter_map.empty() && filter_map.find(thread) == filter_map.end()) {
            continue;
        }
        
        ThreadStack info;
        info.th = thread;
        if ( (options & ThreadOptions::kBasic) == ThreadOptions::kBasic) {
            GetThreadCpuAndName(thread, info.cpu, info.thread_name);
        }
        if ( (options & ThreadOptions::kFrames) == ThreadOptions::kFrames) {
            GetThreadStack(i, thread, info.frames);
        }
        ls.push_back(info);
    }
    return 0;
}

int GetThreadCpuAndName(const thread_t thread, float& cpu, std::string& thread_name) {
    cpu = 0;
    thread_name = "";
    mach_msg_type_number_t thread_info_count = THREAD_INFO_MAX;
    thread_info_data_t thinfo;
    kern_return_t kr = thread_info(thread, THREAD_EXTENDED_INFO, (thread_info_t)thinfo, &thread_info_count);
    if (kr != KERN_SUCCESS) {
        return -1;
    }

    thread_extended_info_t basic_info_th = (thread_extended_info_t)thinfo;
    if (!(basic_info_th->pth_flags & TH_FLAGS_IDLE)) {
        cpu = basic_info_th->pth_cpu_usage / (float)TH_USAGE_SCALE * 100.0;
    }
    thread_name = std::string(basic_info_th->pth_name);
    return 0;
}

std::string GetFrameEntry(const int entryNum, const uintptr_t address) {
    Dl_info dlInfo;
    ksdl_dladdr(address, &dlInfo);
    
    char faddrBuff[20];
    char saddrBuff[20];
    const char* fname = bs_lastPathEntry(dlInfo.dli_fname);
    if(fname == NULL) {
        sprintf(faddrBuff, POINTER_FMT, (uintptr_t)dlInfo.dli_fbase);
        fname = faddrBuff;
    }
    
//    uintptr_t offset = address - (uintptr_t)dlInfo.dli_saddr;
//    const char* sname = dlInfo.dli_sname;
//    const char* pUnused = "<redacted>";
//    if(sname == NULL || 0 == strcmp(sname, pUnused))
//    {
//        sprintf(saddrBuff, POINTER_FMT, (uintptr_t)dlInfo.dli_fbase);
//        sname = saddrBuff;
//        offset = address - (uintptr_t)dlInfo.dli_fbase;
//    }
  
    sprintf(saddrBuff, POINTER_FMT, (uintptr_t)dlInfo.dli_fbase);
    const char * sname = saddrBuff;
    uintptr_t offset = address - (uintptr_t)dlInfo.dli_fbase;
    char buf[2048];
    snprintf(buf, 2048, TRACE_FMT, entryNum, fname, (uintptr_t)address, sname, offset);
    return std::string(buf);
}

int GetCpuCore() {
  int cpuCount = 1;
  size_t len = sizeof(cpuCount);
  int ret = sysctlbyname("hw.physicalcpu", &cpuCount, &len, NULL, 0);
  if (ret != 0) {
    cpuCount = 1;
  }
  return cpuCount;
}
