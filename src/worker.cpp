#include <iostream>
#include <maolan/io.h>
#include <maolan/worker.h>


using namespace maolan;


Worker::Worker() { t = std::thread(&Worker::_process, this); }
Worker::~Worker() { t.join(); }


void Worker::_process()
{
  while (true)
  {
    auto task = IO::task();
    if (task == nullptr)
    {
      break;
    }
    task->work();
  }
}
