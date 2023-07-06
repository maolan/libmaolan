#include <stdexcept>
#include <sys/event.h>

#include "maolan/oss/io.hpp"


using namespace maolan::audio::oss;


std::vector<IO *> IO::_all;


IO::IO(const std::string &device, const bool &audio)
    : maolan::audio::IO{device}, maolan::HW{audio}
{
  _all.push_back(this);
}


IO::~IO()
{
  _all.erase(std::remove(_all.begin(), _all.end(), this), _all.end());
}

maolan::IO *IO::wait() { return nullptr; }
