#pragma once
#include <thread>

namespace maolan
{
class Worker
{
public:
  Worker();
  ~Worker();

protected:
  std::thread t;

private:
  void _process();
};
} // namespace maolan
