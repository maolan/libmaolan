#pragma once
#include <maolan/audio/clip.h>
#include <maolan/audio/connectable.h>


namespace maolan::audio
{
class Track : public IO, public Connectable
{
public:
  Track(const std::string &name, const std::size_t &channel);

  void fetch();
  void process();
  void setup();
  void mute();
  void arm();
  void solo();
  void add(Clip *);
  std::size_t channels() const;
  Buffer pull(const unsigned &channel);

  friend class Clip;

protected:
  bool muted;
  bool armed;
  bool soloed;
  Clip *first;
  Clip *_current;
  Clip *last;
  Clip *recording = nullptr;
};
} // namespace maolan::audio
