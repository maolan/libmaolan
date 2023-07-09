#pragma once
#include "maolan/io.hpp"


struct pollfd;

namespace maolan
{
class HW
{
public:
  static void prepare();
  static IO * wait();
};
} // namespace maolan
