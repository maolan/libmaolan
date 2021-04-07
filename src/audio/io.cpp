#include <iostream>
#include <vector>

#include "maolan/audio/io.hpp"
#include "maolan/config.hpp"


using namespace maolan::audio;


std::vector<maolan::Config *> IO::devices;


IO::IO(const std::string &name, const bool &reg, const size_t &chs)
    : maolan::IO(name, reg)
{
  if (chs > 0)
  {
    _outputs.resize(chs);
  }
}


Buffer IO::pull(const std::size_t &channel)
{
  if (channels() > channel)
  {
    return _outputs[channel];
  }
  std::cerr << _type << ' ' << _name << " has " << channels();
  std::cerr << " channels and channel " << channel + 1 << " requested!\n";
  return nullptr;
}


nlohmann::json IO::json()
{
  auto result = maolan::IO::json();
  result["channels"] = channels();
  return result;
}


std::size_t IO::channels() const { return _outputs.size(); }
