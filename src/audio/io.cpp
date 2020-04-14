#include <iostream>
#include <maolan/audio/io.h>


using namespace maolan::audio;


IO::IO(const size_t &chs, const bool &front, const bool &reg)
    : maolan::IO("", front, reg)
{
  if (chs > 0) { outputs.resize(chs); }
}


Buffer IO::pull(const std::size_t &channel)
{
  if (outputs.size() > channel) { return outputs[channel]; }
  std::cerr << _name << " has " << outputs.size() << " channels and ";
  std::cerr << channel << " channel requested!\n";
  return nullptr;
}
