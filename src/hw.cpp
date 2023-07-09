#include <poll.h>

#include "maolan/audio/hw.hpp"
#include "maolan/hw.hpp"


using namespace maolan;


static std::vector<struct pollfd> pfds;


HW::HW()
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
  }
  _thread = std::thread(&HW::_process, this);
}


IO *HW::wait()
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


void HW::_process()
{
  while (IO::playing() && !IO::quitting())
  {
    auto *hwio = wait();
    hwio->readhw();
    hwio->writehw();
  }
}


HW::~HW() { _thread.join(); }
