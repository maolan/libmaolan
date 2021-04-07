#include "maolan/plugin/io.hpp"


namespace midi = maolan::midi;
namespace audio = maolan::audio;
using namespace maolan::plugin;


IO::IO(const std::string &name, const bool &reg)
    : midi::IO(name, true, reg)
    , audio::IO(0, true, false)
    , midi::Connectable(0)
    , audio::Connectable(0)
{
  midi::IO::_outputs.clear();
  audio::IO::_outputs.clear();
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
  midi::Connectable::fetch();
  audio::Connectable::fetch();
}


void IO::connect(midi::IO *to)
{
  midi::Connectable::connect(to);
}


void IO::connect(midi::IO *to, std::size_t inCh, std::size_t outCh)
{
  midi::Connectable::connect(to, inCh, outCh);
}


void IO::connect(audio::IO *to)
{
  audio::Connectable::connect(to);
}


void IO::connect(audio::IO *to, std::size_t inCh, std::size_t outCh)
{
  audio::Connectable::connect(to, inCh, outCh);
}


nlohmann::json IO::connections() { return conns(audio::IO::_name); }
