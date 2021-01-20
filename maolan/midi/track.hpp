#pragma once
#include "maolan/midi/clip.hpp"
#include "maolan/midi/connectable.hpp"


namespace maolan::midi
{
class Track : public midi::IO, public midi::Connectable
{
public:
  Track(const std::string &name, const std::size_t &channel);

  virtual void fetch();
  virtual void process();
  virtual void setup();
  virtual Buffer pull(const std::size_t &channel);

  void mute(const bool &value = true);
  void arm(const bool &value = true);
  void solo(const bool &value = true);
  void add(midi::Clip *);
  void remove(midi::Clip *);

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
