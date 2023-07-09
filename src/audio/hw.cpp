#include <poll.h>

#include "maolan/audio/hw.hpp"


using namespace maolan::audio;


std::vector<HW *> HW::_all;
static std::vector<struct pollfd> pfds;


HW::HW(const std::string &name) : IO{name}
{
  _all.push_back(this);
}


HW::~HW()
{
  (void)std::remove(_all.begin(), _all.end(), this);
}


int HW::fd() { return _fd; }
const std::vector<HW *> & HW::all() { return _all; }
