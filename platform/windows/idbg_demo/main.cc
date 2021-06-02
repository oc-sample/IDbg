//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include <iostream>
#include "third_party/IDbg/include/thread_model.h"
#include "third_party/IDbg/include/sys_util.h"
#include <windows.h>

//#pragma comment ( lib, "third_party/IDbg/lib/IDbg.lib" )

#include <Windows.h> 
#include <iostream>
#include <processthreadsapi.h>



extern void printCallStark(); 

#define COUNT 1
void func() {
  //printCallStark();
  for (size_t i = 0; i < COUNT; ++i) {
    int * pInt = (int*)malloc(sizeof(int));
    //free(pInt);
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
  //HeapProfilerStart();
  Sleep(20000);

  printf("start alloc");
  TestFunc();

  /*IDbg::ThreadModel th{ "demo_thread" };
  th.PushTask([] {
    std::cout << "in thread model " << std::this_thread::get_id() << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::seconds(3));*/

  //HeapProfilerStop();
  //_CrtDumpMemoryLeaks();

  printf("finish alloc");

  //IDbg::SetThreadName("main");

  Sleep(20000);
}
