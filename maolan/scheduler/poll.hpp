#pragma once
#include <thread>

#include <maolan/io.hpp>
#include <maolan/scheduler/base.hpp>

namespace maolan::scheduler {
class Poll : BaseScheduler {
public:
  Poll();

  static IO *wait();

protected:
  void _process();
};
} // namespace maolan::scheduler
