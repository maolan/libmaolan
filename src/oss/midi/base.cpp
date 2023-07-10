#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#include "maolan/midi/event.hpp"
#include "maolan/oss/midi/base.hpp"

using namespace maolan::midi;


OSS::OSS(const std::string &name) : HW{name}
{
  _data = std::make_shared<BufferData>();
  if ((_fd = open(_name.data(), O_RDWR | O_NONBLOCK, 0)) == -1)
  {
    std::cerr << _name << ' ' << std::strerror(errno) << '\n';
  }
  _all.push_back(this);
}


OSS::~OSS()
{
  (void)std::remove(_all.begin(), _all.end(), this);
  close(_fd);
}
