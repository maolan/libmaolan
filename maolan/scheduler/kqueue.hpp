#pragma once
#include <thread>

#include <maolan/io.hpp>
#include <maolan/scheduler/base.hpp>

namespace maolan::scheduler {
class KQueue : BaseScheduler {
public:
  KQueue();

  static IO *wait();

protected:
  void _process();
};
} // namespace maolan::scheduler
