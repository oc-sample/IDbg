//
// Created by mjzheng on 2020/6/12.
//

#ifndef ALGORITHM_SINGLETON_H
#define ALGORITHM_SINGLETON_H

#include <mutex>
#include <thread>

namespace IDbg {

class NonCopyable {
 protected:
  virtual ~NonCopyable() = default;
  NonCopyable() = default;
 private:
  NonCopyable(const NonCopyable& other) = delete;
  NonCopyable(NonCopyable&& other) = delete;
  NonCopyable& operator=(const NonCopyable& other) = delete;
  NonCopyable& operator=(NonCopyable&& other) = delete;
};

template <typename T>
class Singleton : public NonCopyable {
 public:
  static T* Instance();
 protected:
  virtual ~Singleton() = default;
  Singleton() = default;
};

template <typename T>
T * Singleton<T>::Instance() {
  static T instance;
  return &instance;
}

}  // namespace IDbg

#endif //ALGORITHM_SINGLETON_H
