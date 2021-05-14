//
// Created by mjzheng on 2021/3/18.
// Copyright (c) 2021 zheng jun ming. All rights reserved.
//

#include "thread_model.h"
#include "sys_util.h"
#include "common_def.h"


namespace IDbg {
ThreadModel::ThreadModel(const std::string& thread_name)
: quited_(false), thread_name_(thread_name) {
  StartThread();
  LOG("construct");
}

ThreadModel::~ThreadModel() {
  StopThread();
  LOG("destroy...");
}

int ThreadModel::PushTask(std::function<void()> func) {
  std::lock_guard<std::mutex> locker(mu_);
  task_list_.push(func);
  cv_.notify_one();
  return 0;
}

int ThreadModel::StartThread() {
  LOG("begin");
  th_ = std::unique_ptr<std::thread>(new std::thread([this] {
    Run();
  }));
  LOG("end");
  return 0;
}

int ThreadModel::StopThread() {
  LOG("begin");
  {
    std::lock_guard<std::mutex> guard(mu_);
    quited_ = true;
    cv_.notify_one();
  }
  if (th_ != nullptr && th_->joinable()) {
    th_->join();
  }
  LOG("end");
  return 0;
}

void ThreadModel::DoAllTask() {
  while (true) {
    std::function<void()> func = nullptr;
    {
      std::lock_guard<std::mutex> guard(mu_);
      if (task_list_.empty() || quited_) {
          break;
      }
      func = task_list_.front();
      task_list_.pop();
    }
    if (func != nullptr) {
      func();
    }
  }
}

void ThreadModel::Run() {
  LOG(" begin");
  SetThreadName(thread_name_);
  while (true) {
    DoAllTask();
    std::unique_lock<std::mutex> locker(mu_);
    cv_.wait(locker, [this] {
      return quited_ || !task_list_.empty();
    });
    if (quited_) {
      break;
    }
  }
  LOG("leave");
  return;
}

int ThreadModel::Sleep(int64_t interval) {
  std::this_thread::sleep_for(std::chrono::milliseconds(interval));
  std::lock_guard<std::mutex> guard(mu_);
  return quited_ ? 1 : 0;
}

}  // namespace IDbg
