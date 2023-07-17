#include "maolan/midi/hw.hpp"


using namespace maolan::midi;


std::vector<HW *> HW::_all;


HW::HW(const std::string &name, const std::string &device)
  : IO{name}
  , _device{device}
{
  _all.push_back(this);
}
HW::~HW() { (void)std::remove(_all.begin(), _all.end(), this); }
int HW::fd() { return _fd; }
std::string HW::device() { return _device; }
const std::vector<HW *> &HW::all() { return _all; }
