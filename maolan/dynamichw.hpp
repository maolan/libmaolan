#pragma once
#include <string>

#include "maolan/audio/hw.hpp"
#include "maolan/midi/hw.hpp"


namespace maolan
{
class DynamicHW
{
  public:
    std::map<std::string, audio::audio_t> audio;
    std::map<std::string, midi::midi_t> midi;
};
}
