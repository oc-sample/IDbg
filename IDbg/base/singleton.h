//
// Created by mjzheng on 2020/6/12.
//

#ifndef ALGORITHM_SINGLETON_H
#define ALGORITHM_SINGLETON_H

#include <mutex>
#include <thread>

class Singleton {
private:
  Singleton() = default;  // 构造函数私有化

  Singleton(const Singleton& other) =delete;  // 禁止拷贝构造函数

  Singleton& operator=(const Singleton& other) = delete;  // 禁止赋值函数

public:
  ~Singleton() = default;

  static Singleton* GetInstance() {
    if (nullptr == m_singleton) {
      std::lock_guard<std::mutex> lock(m_mutex);
      if (nullptr == m_singleton) {
        m_singleton = new Singleton();
      }
    }
    return m_singleton;
  }

  static Singleton* GetInstance2() {
    static Singleton instance;
    return &instance;
  }

private:
  static Singleton* m_singleton;
  static std::mutex m_mutex;
};

Singleton* Singleton::m_singleton = nullptr;
std::mutex Singleton::m_mutex;

void TestSingleton() {
  Singleton* p1 = Singleton::GetInstance();
  Singleton* p2 = Singleton::GetInstance();
  if (p1 == p2) {
    std::cout << "same" << std::endl;
  }

  Singleton* p3 = Singleton::GetInstance2();
  Singleton* p4 = Singleton::GetInstance2();
  if (p3 == p4) {
    std::cout << "same" << std::endl;
  }
}


#endif //ALGORITHM_SINGLETON_H
