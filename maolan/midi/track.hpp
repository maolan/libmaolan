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
  virtual Buffer pull(const std::size_t &channel);

  void mute(const bool &value = true);
  void arm(const bool &value = true);
  void solo(const bool &value = true);
  void add(midi::Clip *);
  void remove(midi::Clip *);
  Clip *clips();

protected:
  bool muted;
  bool armed;
  bool soloed;
  std::size_t _channel;
  midi::Clip *first;
  midi::Clip *_current;
  midi::Clip *last;
  midi::Clip *recording;
};
} // namespace maolan::midi
