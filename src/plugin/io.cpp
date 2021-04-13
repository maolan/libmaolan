#include "maolan/plugin/io.hpp"


namespace midi = maolan::midi;
namespace audio = maolan::audio;
using namespace maolan::plugin;


IO::IO(const std::string &name, const bool &reg)
    : audio::IO(name, reg), midi::IO(name, reg)
{
}


maolan::audio::Buffer IO::audio(const std::size_t &channel)
{
  if (channel < audio::IO::_outputs.size())
  {
    return audio::IO::_outputs[channel];
  }
  return nullptr;
}


maolan::midi::Buffer IO::midi(const std::size_t &channel)
{
  if (channel < midi::IO::_outputs.size())
  {
    return midi::IO::_outputs[channel];
  }
  return nullptr;
}


void IO::fetch()
{
  midi::IO::fetch();
  audio::IO::fetch();
}


void IO::connect(midi::IO *to, std::size_t inCh, std::size_t outCh)
{
  midi::IO::connect(to, inCh, outCh);
}


void IO::connect(audio::IO *to, std::size_t inCh, std::size_t outCh)
{
  audio::IO::connect(to, inCh, outCh);
}


void IO::connect(audio::IO *to) { audio::IO::connect(to); }
void IO::connect(midi::IO *to) { midi::IO::connect(to); }
