#include "maolan/plugin/io.hpp"


using namespace maolan::plugin;


IO::IO(const std::string &name, const bool &reg)
    : audio::IO(name, reg), midi::IO(name, reg)
{}


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
  return midi::IO::_output;
}


void IO::fetch()
{
  midi::IO::fetch();
  audio::IO::fetch();
}


void IO::connect(audio::IO *to, std::size_t inch, std::size_t outch)
{
  audio::IO::connect(to, inch, outch);
}


void IO::connect(audio::IO *to) { audio::IO::connect(to); }
void IO::connect(midi::IO *to) { midi::IO::connect(to); }
