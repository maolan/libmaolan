#pragma once
#include "maolan/audio/clip.hpp"
#include "maolan/audio/io.hpp"

#ifdef LV2_ENABLED
#include "maolan/plugin/lv2/plugin.hpp"
#endif


namespace maolan::audio
{
class Track : public IO
{
public:
  Track(const std::string &name, const std::size_t &channel);
  virtual ~Track();

  static std::vector<Track *> _all;

  virtual void init();
  virtual void setup();
  virtual void fetch();
  virtual void process();
  virtual nlohmann::json json();
  virtual std::size_t channels() const;
  virtual Buffer pull(const std::size_t &channel = 0);

  bool mute();
  bool arm();
  bool solo();
  void mute(const bool &value);
  void arm(const bool &value);
  void solo(const bool &value);

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
  Clip *_first;
  Clip *_current;
  Clip *_last;
  Clip *_recording = nullptr;
#ifdef LV2_ENABLED
  std::vector<plugin::lv2::Plugin *> _plugins;
#endif
};
} // namespace maolan::audio
