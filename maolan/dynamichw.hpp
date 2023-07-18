#pragma once
#include <string>

#include "maolan/audio/hw.hpp"
#include "maolan/midi/hw.hpp"


namespace maolan
{
class Audio
{
  public:
    audio::audio_t in;
    audio::audio_t out;
};


class Midi
{
  public:
    midi::midi_t in;
    midi::midi_t out;
};


class DynamicHW
{
  public:
    DynamicHW(const std::string &lib);

    Audio audio;
    Midi midi;
};
}
