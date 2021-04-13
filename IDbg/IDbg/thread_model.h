//
// Created by mjzheng on 2021/3/18.
//

#ifndef C11_SAMPLE_THREADMODEL_H
#define C11_SAMPLE_THREADMODEL_H

#include <thread>
#include <iostream>
#include <queue>
#include <string>

namespace IDbg {

class ThreadModel {
public:
  explicit ThreadModel(const std::string& thread_name);

  ~ThreadModel();

public:
  int PushTask(std::function<void()> func);
  
  int Sleep(int64_t interval);

protected:
  void Run();

  void DoAllTask();

private:
  int StartThread();

  int StopThread();
  
  void SetThreadName(const std::string& name);

private:
  bool quited_;
  std::condition_variable cv_;
  std::mutex mu_;
  std::queue<std::function<void()>> task_list_;

private:
  std::unique_ptr<std::thread> th_;
  std::string thread_name_;
};

} // namscpace IDbg

#endif //C11_SAMPLE_THREADMODEL_H
