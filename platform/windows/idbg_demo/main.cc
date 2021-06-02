//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include <iostream>
#include "third_party/IDbg/include/thread_model.h"
#include "third_party/IDbg/include/sys_util.h"
#include <windows.h>

#define COUNT 1

void manual_alloc() {
  for (size_t i = 0; i < COUNT; ++i) {
    int * pInt = (int*)malloc(sizeof(int));
    //free(pInt);
  }
}

void TestFunc() {
  int start_flag = 0;
  scanf_s("%d", &start_flag);

  manual_alloc();

  int finish_flag = 0;
  scanf_s("%d", &finish_flag);

  //_CrtDumpMemoryLeaks();
}

int main(int argc, char* argv[]) {
  TestFunc();

  /*IDbg::ThreadModel th{ "demo_thread" };
  th.PushTask([] {
    std::cout << "in thread model " << std::this_thread::get_id() << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::seconds(3));

  IDbg::SetThreadName("main");*/
  //system("pause");
}
