#pragma once
#include <thread>

namespace maolan::scheduler {
class BaseScheduler {
public:
  virtual ~BaseScheduler();

  virtual void join();

protected:
  BaseScheduler();

  std::thread _thread;
};
} // namespace maolan::scheduler
