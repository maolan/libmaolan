#pragma once
#include <maolan/audio/track.hpp>
#include <maolan/midi/track.hpp>
#include <maolan/plugin/io.hpp>

namespace maolan::plugin {
class Instrument {
public:
  Instrument(const std::string &name, const std::string &type);

  maolan::audio::Track *audio();
  maolan::midi::Track *midi();

  void connect(maolan::audio::IO *to);
  void connect(maolan::midi::IO *to);

protected:
  maolan::audio::Track *_audio;
  maolan::midi::Track *_midi;
  plugin::IO *_plugin;
};
} // namespace maolan::plugin
