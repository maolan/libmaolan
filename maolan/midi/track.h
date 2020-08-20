#pragma once
#include <maolan/midi/clip.h>
#include <maolan/midi/connectable.h>


namespace maolan::midi
{
class Track : public IO, public Connectable
{
public:
  Track(const std::string &name, const std::size_t &channel);

  virtual void fetch();
  virtual void process();
  virtual void setup();
  void mute(const bool &value = true);
  void arm(const bool &value = true);
  void solo(const bool &value = true);
  void add(Clip *);
  void remove(Clip *);
  virtual Buffer pull();

protected:
  bool muted;
  bool armed;
  bool soloed;
  std::size_t _channel;
  Clip *first;
  Clip *_current;
  Clip *last;
  Clip *recording;
};
} // namespace maolan::midi
