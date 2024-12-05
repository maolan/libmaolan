#pragma once
#include <vector>

#include <maolan/audio/clip.hpp>
#include <maolan/audio/io.hpp>

#ifdef LV2_ENABLED
#include "maolan/plugin/lv2/plugin.hpp"
#endif

namespace maolan::audio {
class Track : public IO {
public:
  Track(const std::string &name, const size_t &channel);
  virtual ~Track();

  virtual void init();
  virtual void setup();
  virtual void fetch();
  virtual void process();
  virtual nlohmann::json json();
  virtual Buffer pull(const size_t &channel = 0);

  bool mute();
  bool arm();
  bool solo();
  float volume();
  void mute(const bool &value);
  void arm(const bool &value);
  void solo(const bool &value);
  void volume(const float &value);

  void add(Clip *);
  void remove(Clip *);
#ifdef LV2_ENABLED
  void add(plugin::lv2::Plugin *);
  void remove(plugin::lv2::Plugin *);
#endif
  Clip *clips();

protected:
  bool _muted;
  bool _armed;
  bool _soloed;
  float _volume;
  Clip *_first;
  Clip *_current;
  Clip *_last;
  Clip *_recording;
#ifdef LV2_ENABLED
  std::vector<plugin::lv2::Plugin *> _plugins;
#endif
};
} // namespace maolan::audio
