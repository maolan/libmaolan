#pragma once
#include <thread>

#include "maolan/io.hpp"


namespace maolan
{
class HW
{
public:
  HW();
  ~HW();

  static IO *wait();

protected:
  std::thread _thread;
  void _process();
};
} // namespace maolan
