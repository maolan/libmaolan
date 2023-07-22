#include <poll.h>

#include "maolan/audio/hw.hpp"
#include "maolan/midi/hw.hpp"
#include "maolan/scheduler/poll.hpp"


using namespace maolan::scheduler;


static std::vector<struct pollfd> pfds;


Poll::Poll()
{
  struct pollfd pfd;
  const auto &audiohw = audio::HW::all();
  const auto &midihw = midi::HW::all();
  pfds.clear();
  pfds.reserve(audiohw.size() + midihw.size());
  for (const auto &hw : audiohw)
  {
    pfd.fd = hw->fd();
    pfd.events = POLLIN | POLLOUT;
    pfd.revents = 0;
    pfds.push_back(pfd);
    hw->readhw();
    hw->process();
  }
  for (const auto &hw : midihw)
  {
    pfd.fd = hw->fd();
    pfd.events = POLLIN | POLLOUT;
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
  if (ret == -1) { return nullptr; }
  for (const auto &pfd : pfds)
  {
    if (pfd.revents != 0)
    {
      for (const auto &hw : audio::HW::all())
      {
        if (hw->fd() == pfd.fd) { return hw; }
      }
      for (const auto &hw : midi::HW::all())
      {
        if (hw->fd() == pfd.fd) { return hw; }
      }
    }
  }
  return nullptr;
}


void Poll::_process()
{
  size_t pindex = 0;
  const auto &audiohw = audio::HW::all();

  while (IO::playing() && !IO::quitting())
  {
    auto *hwio = wait();
    hwio->readhw();
    hwio->writehw();
    if (std::find(audiohw.begin(), audiohw.end(), hwio) != audiohw.end())
    {
      ++pindex;
      if (pindex >= audiohw.size())
      {
        pindex = 0;
        IO::tick();
      }
    }
  }
}


Poll::~Poll() { _thread.join(); }
