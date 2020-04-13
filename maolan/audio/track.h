#pragma once
#include <maolan/audio/clip.h>
#include <maolan/audio/connectable.h>
#include <maolan/audio/plugin.h>


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
  void remove(Clip *);
  void add(Plugin *);
  void remove(Plugin *);
  std::size_t channels() const;
  virtual Buffer pull(const std::size_t &channel);

protected:
  bool muted;
  bool armed;
  bool soloed;
  Clip *first;
  Clip *_current;
  Clip *last;
  Clip *recording = nullptr;
  std::vector<Plugin *> _plugins;
};
} // namespace maolan::audio
