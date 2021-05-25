#include <iostream>
#include "third_party/IDbg/include/thread_model.h"
#include "third_party/IDbg/include/sys_util.h"

//#pragma comment ( lib, "third_party/IDbg/lib/IDbg.lib" )

#include <Windows.h> 
#include <iostream>

#define COUNT 1000*1000
void func() {
  for (size_t i = 0; i < COUNT; ++i) {
    int * pInt = (int*)malloc(i * sizeof(int));
    free(pInt);
  }
}

void TestFunc() {
  DWORD tStart, tEnd;

  tStart = timeGetTime();
  func();
  tEnd = timeGetTime();
  printf("%lu\n", tEnd - tStart);
}

int main(int argc, char* argv[]) {
  TestFunc();
  /*std::cout << "mjzheng" << std::endl;

  IDbg::SetThreadName("main");

  IDbg::ThreadModel th{ "demo_thread" };
  th.PushTask([] {
    std::cout << "in thread model " << std::this_thread::get_id() << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::seconds(3));*/

  system("pause...");
}
