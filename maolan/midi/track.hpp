#pragma once
#include <nlohmann/json.hpp>

#include "maolan/midi/clip.hpp"
#include "maolan/midi/io.hpp"


namespace maolan::midi
{
class Track : public midi::IO
{
public:
  Track(const std::string &name, const std::size_t &channel);

  virtual void fetch();
  virtual void process();
  virtual void setup();
  virtual nlohmann::json json();
  virtual Buffer pull();

  void mute(const bool &value = true);
  void arm(const bool &value = true);
  void solo(const bool &value = true);
  void add(midi::Clip *);
  void remove(midi::Clip *);
  Clip *clips();

protected:
  bool _muted;
  bool _armed;
  bool _soloed;
  std::size_t _channel;
  midi::Clip *_first;
  midi::Clip *_current;
  midi::Clip *_last;
  midi::Clip *_recording;
};
} // namespace maolan::midi
