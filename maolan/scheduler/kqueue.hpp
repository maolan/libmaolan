#pragma once
#include <thread>

#include "maolan/io.hpp"


namespace maolan::scheduler
{
class KQueue
{
public:
  KQueue();
  ~KQueue();

  static IO *wait();

protected:
  std::thread _thread;
  void _process();
};
} // namespace maolan

