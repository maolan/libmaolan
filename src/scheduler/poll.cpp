#include <atomic>
#include <poll.h>

#include "maolan/audio/hw.hpp"
#include "maolan/scheduler/poll.hpp"


using namespace maolan::scheduler;


static std::vector<struct pollfd> pfds;


Poll::Poll()
{
  struct pollfd pfd;
  const auto &audiohw = audio::HW::all();
  pfds.clear();
  pfds.reserve(audiohw.size());
  for (const auto &hw : audiohw)
  {
    pfd.fd = hw->fd();
    pfd.events = POLLIN;
    pfd.revents = 0;
    pfds.push_back(pfd);
    hw->readhw();
    hw->process();
  }
  _thread = std::thread(&Poll::_process, this);
}


maolan::IO *Poll::wait()
{
  int ret = poll(pfds.data(), pfds.size(), -1);
  if (ret == -1)
  {
    return nullptr;
  }
  for (const auto &pfd : pfds)
  {
    if (pfd.revents != 0)
    {
      for (const auto &hw : audio::HW::all())
      {
        if (hw->fd() == pfd.fd)
        {
          return hw;
        }
      }
    }
  }
  return nullptr;
}


void Poll::_process()
{
  std::atomic_size_t pindex = 0;
  const size_t size = pfds.size();
  while (IO::playing() && !IO::quitting())
  {
    auto *hwio = wait();
    hwio->readhw();
    hwio->writehw();
    if (size == 1)
    {
      pindex = 0;
    }
    else
    {
      pindex = ++pindex / size;
    }
    if (pindex == 0)
    {
      IO::tick();
    }
  }
}


Poll::~Poll() { _thread.join(); }
