
#include <iostream>

#include "third_party/IDbg/include/thread_model.h"
#include "third_party/IDbg/include/sys_util.h"


extern void TestMemory();

int main(int argc, char* argv[]) {
  TestMemory();

  //SYSTEM_INFO sys_info;
  //GetSystemInfo(&sys_info);
  //int j = 0;

  //MEMORYSTATUS ms = { sizeof(ms) };
  //GlobalMemoryStatus(&ms);
  //int k = 0;

  /*IDbg::ThreadModel th{ "demo_thread" };
  th.PushTask([] {
    std::cout << "in thread model " << std::this_thread::get_id() << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::seconds(3));

  IDbg::SetThreadName("main");*/
  //system("pause");
}
