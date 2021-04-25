//
//  ThreadInfo.m
//  AppleMiniDumpApp
//
//  Created by mjzheng on 2019/6/28.
//  Copyright © 2019年 mjzheng. All rights reserved.
//

#include "thread_cpu_info.h"

#include <sys/sysctl.h>
#include <pthread/pthread.h>

#include "ks_dynamic_linker.h"

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

namespace IDbg {

void GetFrameEntry(const int entry_num, const uintptr_t address, FrameInfo* frame);

int GetThreadCpuAndName(const thread_t thread, float* cpu, std::string* thread_name);

typedef struct ana_cpu_load_info {
  natural_t cpu_user;
  natural_t cpu_system;
  natural_t cpu_idle;
  natural_t cpu_nice;
}ana_cpu_load_info;

float GetSysCpu() {
  ana_cpu_load_info cpuinfo;
  static struct ana_cpu_load_info lastcpuinfo = {0, 0, 0, 0};
  mach_port_t host_port;
  mach_msg_type_number_t host_size;
  host_cpu_load_info_data_t host_load;
  host_port = mach_host_self();
  host_size = sizeof(host_cpu_load_info_data_t) / sizeof(integer_t);
  int ret = host_statistics(host_port, HOST_CPU_LOAD_INFO, (host_info_t)&host_load, &host_size);
  if (KERN_SUCCESS != ret) {
    return -1;
  } else {
    cpuinfo.cpu_user = host_load.cpu_ticks[0] - lastcpuinfo.cpu_user;
    cpuinfo.cpu_system = host_load.cpu_ticks[1] - lastcpuinfo.cpu_system;
    cpuinfo.cpu_idle = host_load.cpu_ticks[2] - lastcpuinfo.cpu_idle;
    cpuinfo.cpu_nice = host_load.cpu_ticks[3] - lastcpuinfo.cpu_nice;
    memcpy(&lastcpuinfo, &host_load, sizeof(lastcpuinfo));
  }
  return 100.0*((float)(cpuinfo.cpu_user + cpuinfo.cpu_system) / (float)(cpuinfo.cpu_user + cpuinfo.cpu_system + cpuinfo.cpu_idle + cpuinfo.cpu_nice));
}

typedef struct StackFrameEntry {
  const struct StackFrameEntry *const previous;  //低地址
  const uintptr_t return_address;  //高地址
} StackFrameEntry;

#pragma -mark HandleMachineContext
bool thread_get_state_ex(thread_t thread, _STRUCT_MCONTEXT *machineContext) {
  mach_msg_type_number_t state_count = THREAD_STATE_COUNT;
  kern_return_t kr = thread_get_state(thread, THREAD_STATE, (thread_state_t)&machineContext->__ss, &state_count);
  return (kr == KERN_SUCCESS);
}

uintptr_t mach_instructionAddress(mcontext_t const machine_context) {
  return machine_context->__ss.INSTRUCTION_ADDRESS;
}

uintptr_t mach_linkRegister(mcontext_t const machine_context) {
#if defined(__i386__) || defined(__x86_64__)
  return 0;
#else
  return machine_context->__ss.__lr;
#endif
}

uintptr_t mach_framePointer(mcontext_t const machine_context) {
  return machine_context->__ss.FRAME_POINTER;
}

uintptr_t mach_stackPointer(mcontext_t const machine_context) {
  return machine_context->__ss.STACK_POINTER;
}

kern_return_t mach_copyMem(const void *const src, void *const dst, const size_t numBytes) {
    vm_size_t bytes_copied = 0;
    return vm_read_overwrite(mach_task_self(), (vm_address_t)src, (vm_size_t)numBytes, (vm_address_t)dst, &bytes_copied);
}

const char* bs_lastPathEntry(const char* const path) {
  if (path == NULL) {
    return NULL;
  }
  const char* last_file = strrchr(path, '/');
  return last_file == NULL ? path : last_file + 1;
}

// C function
int GetThreadStack(thread_t thread, FrameList& frame_list) {
  // 获取线程的ebp 和esp, 用于调用链重构
  _STRUCT_MCONTEXT ctx;
  if (!thread_get_state_ex(thread, &ctx)) {
    return -1;
  }
  const uintptr_t instructionAddress = mach_instructionAddress(&ctx);
  if (instructionAddress == 0) {
    return -1;
  }

  int i = 0;
  uintptr_t backtrace_buffer[MAX_BACKTRACE];
  memset(backtrace_buffer, 0, sizeof(backtrace_buffer));
  backtrace_buffer[i++] = instructionAddress;
  uintptr_t link_register = mach_linkRegister(&ctx);
  if (link_register) {
    backtrace_buffer[i++] = link_register;
  }

  StackFrameEntry frame = {0};
  const uintptr_t frame_ptr = mach_framePointer(&ctx);

  if (frame_ptr == 0 || mach_copyMem((void*)frame_ptr, &frame, sizeof(frame)) != KERN_SUCCESS) {
    return -1;
  }

  for (; i < MAX_BACKTRACE; i++) {
    backtrace_buffer[i] = frame.return_address;
    if (backtrace_buffer[i] == 0 || frame.previous == 0 || mach_copyMem(frame.previous, &frame, sizeof(frame)) != KERN_SUCCESS) {
        break;
    }
  }

  for (int j=0; j < i; j++) {
    FrameInfo frame;
    GetFrameEntry(j, backtrace_buffer[j], &frame);
    frame_list.push_back(frame);
  }
  return 0;
}

int GetThreadArray(const ThreadOptions options, const thread_act_array_t threads, const mach_msg_type_number_t thread_count, ThreadStackArray& ls) {
  ThreadIdArray id_ls;
  for (int i=0; i < thread_count; ++i) {
    id_ls.push_back(threads[i]);
  }
  return GetThreadInfoById(ls, id_ls, options);
}

int GetThreadInfoById(ThreadStackArray& ls, const ThreadIdArray& id_ls, const ThreadOptions options) {
  ls.clear();
  if ((options&ThreadOptions::kFrames) == ThreadOptions::kFrames) {
    for (auto& thread : id_ls) {
      ThreadStack info;
      info.th = thread;
      int ret = GetThreadCpuAndName(thread, &info.cpu, &info.name);
      if (ret != 0) {
        continue;
      }
      GetThreadStack(thread, info.frames);
      ls.push_back(info);
    }
  } else {
    for (auto& thread : id_ls) {
      ThreadStack info;
      info.th = thread;
      int ret = GetThreadCpuAndName(thread, &info.cpu, &info.name);
      if (ret != 0) {
        continue;
      }
      ls.push_back(info);
    }
  }
  return 0;
}

int GetAllThreadInfo(ThreadStackArray& ls, const ThreadOptions options) {
  thread_act_array_t threads;
  mach_msg_type_number_t thread_count = 0;
  const task_t this_task = mach_task_self();
  kern_return_t kr = task_threads(this_task, &threads, &thread_count);
  if (kr != KERN_SUCCESS) {
    return -1;
  }
  GetThreadArray(options, threads, thread_count, ls);
  kr = vm_deallocate(mach_task_self(), (vm_offset_t)threads, thread_count * sizeof(thread_t));
  assert(kr == KERN_SUCCESS);
  return 0;
}

float GetAppCpu() {
  float app_cpu = 0;
  thread_act_array_t threads;
  mach_msg_type_number_t thread_count = 0;
  const task_t this_task = mach_task_self();
  kern_return_t kr = task_threads(this_task, &threads, &thread_count);
  if (kr != KERN_SUCCESS) {
    return -1;
  }

  for (int i=0; i < thread_count; i++) {
    thread_t thread = threads[i];
    float th_cpu = 0;
    mach_msg_type_number_t thread_info_count = THREAD_INFO_MAX;
    thread_info_data_t thinfo;
    kern_return_t kr = thread_info(thread, THREAD_EXTENDED_INFO, (thread_info_t)thinfo, &thread_info_count);
    if (kr != KERN_SUCCESS) {
      kr = vm_deallocate(mach_task_self(), (vm_offset_t)threads, thread_count * sizeof(thread_t));
      assert(kr == KERN_SUCCESS);
      return -1;
    }

    thread_extended_info_t basic_info_th = (thread_extended_info_t)thinfo;
    if (!(basic_info_th->pth_flags & TH_FLAGS_IDLE)) {
      th_cpu = basic_info_th->pth_cpu_usage / static_cast<float>(TH_USAGE_SCALE);
      app_cpu += th_cpu*100;
    }
  }
  kr = vm_deallocate(mach_task_self(), (vm_offset_t)threads, thread_count * sizeof(thread_t));
  assert(kr == KERN_SUCCESS);
  return app_cpu;
}

int GetThreadCpuAndName(const thread_t thread, float* cpu, std::string* thread_name) {
  *cpu = 0;
  *thread_name = "";
  mach_msg_type_number_t thread_info_count = THREAD_INFO_MAX;
  thread_info_data_t thinfo;
  kern_return_t kr = thread_info(thread, THREAD_EXTENDED_INFO, (thread_info_t)thinfo, &thread_info_count);
  if (kr != KERN_SUCCESS) {
    return -1;
  }

  thread_extended_info_t basic_info_th = (thread_extended_info_t)thinfo;
  if (!(basic_info_th->pth_flags & TH_FLAGS_IDLE)) {
    *cpu = basic_info_th->pth_cpu_usage / static_cast<float>(TH_USAGE_SCALE) * 100.0;
  }
  *thread_name = std::string(basic_info_th->pth_name);
  return 0;
}

void GetFrameEntry(const int entry_num, const uintptr_t address,
                   FrameInfo* frame) {
  if (frame == nullptr) {
    return;
  }
  frame->index = entry_num;
  frame->address = address;

  Dl_info dl_info = {0};
  ksdl_dladdr(address, &dl_info);

  // set module base
  frame->module_base = (uintptr_t)dl_info.dli_fbase;

  // set module name, use module base as module name if module name is empty
  frame->module_name = "";
  const char* fname = bs_lastPathEntry(dl_info.dli_fname);
  if (fname == NULL) {
    if (dl_info.dli_fbase != NULL) {
        frame->module_name = std::to_string((uintptr_t)dl_info.dli_fbase);
    }
  } else {
    frame->module_name = fname;
  }

  // set func_name and offset
  if (dl_info.dli_sname != NULL && dl_info.dli_saddr != NULL) {
    frame->func_name = dl_info.dli_sname;
    frame->offset = address - (uintptr_t)dl_info.dli_saddr;
  }
  if (frame->func_name == "" || frame->func_name == "<redacted>") {
    char saddrBuff[20];
    snprintf(saddrBuff, sizeof(saddrBuff), POINTER_FMT, frame->module_base);
    frame->func_name = saddrBuff;
    frame->offset = address - frame->module_base;
  }
}

int GetCpuCore() {
  int cpu_count = 1;
  size_t len = sizeof(cpu_count);
  int ret = sysctlbyname("hw.physicalcpu", &cpu_count, &len, NULL, 0);
  if (ret != 0) {
    cpu_count = 1;
  }
  return cpu_count;
}

}  // namespace IDbg
