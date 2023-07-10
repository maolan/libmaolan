#pragma once
#include <thread>

#include "maolan/io.hpp"


namespace maolan::scheduler
{
class Poll
{
public:
  Poll();
  ~Poll();

  static IO *wait();

protected:
  std::thread _thread;
  void _process();
};
} // namespace maolan

