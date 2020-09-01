#pragma once
#include <maolan/audio/clip.h>
#include <maolan/audio/connectable.h>
#include <maolan/plugin/lv2/plugin.h>


namespace maolan::audio
{
class Track : public audio::IO, public audio::Connectable
{
public:
  Track(const std::string &name, const std::size_t &channel);

  virtual void fetch();
  virtual void process();
  virtual void setup();
  virtual std::size_t channels() const;
  virtual Buffer pull(const std::size_t &channel);

  void mute(const bool &value = true);
  void arm(const bool &value = true);
  void solo(const bool &value = true);
  void add(Clip *);
  void remove(Clip *);
  void add(plugin::lv2::Plugin *);
  void remove(plugin::lv2::Plugin *);

protected:
  bool muted;
  bool armed;
  bool soloed;
  Clip *first;
  Clip *_current;
  Clip *last;
  Clip *recording = nullptr;
  std::vector<plugin::lv2::Plugin *> _plugins;
};
} // namespace maolan::audio
