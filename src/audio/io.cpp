#include <iostream>
#include <vector>
#include <maolan/config.h>
#include <maolan/audio/io.h>


using namespace maolan::audio;


std::vector<maolan::Config *> IO::devices;


IO::IO(const size_t &chs, const bool &front, const bool &reg, const std::string &name)
  : maolan::IO(name, front, reg)
{
  if (chs > 0) { outputs.resize(chs); }
}


Buffer
IO::pull(const std::size_t &channel)
{
  if (channels() > channel) { return outputs[channel]; }
  std::cerr << _type << ' ' << _name << " has " << channels();
  std::cerr << " channels and channel " << channel + 1 << " requested!\n";
  return nullptr;
}
