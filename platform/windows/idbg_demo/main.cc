#include <iostream>
#include "third_party/IDbg/include/thread_model.h"
#include "third_party/IDbg/include/sys_util.h"

//#pragma comment ( lib, "third_party/IDbg/lib/IDbg.lib" )

int main(int argc, char* argv[]) {
  std::cout << "mjzheng" << std::endl;

  IDbg::SetThreadName("main");

  IDbg::ThreadModel th{ "demo_thread" };
  th.PushTask([] {
    std::cout << "in thread model " << std::this_thread::get_id() << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::seconds(3));

  system("pause");
}
