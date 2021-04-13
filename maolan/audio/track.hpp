#pragma once
#include "maolan/audio/clip.hpp"
#include "maolan/audio/io.hpp"
#include "maolan/plugin/lv2/plugin.hpp"


namespace maolan::audio
{
class Track : public IO
{
public:
  Track(const std::string &name, const std::size_t &channel);
  virtual ~Track();

  static std::vector<Track *> all;

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
  void add(plugin::lv2::Plugin *);
  void remove(plugin::lv2::Plugin *);
  Clip *clips();

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
