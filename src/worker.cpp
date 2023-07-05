#include "maolan/worker.hpp"
#include "maolan/io.hpp"


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
