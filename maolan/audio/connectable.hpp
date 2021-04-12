#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "maolan/audio/input.hpp"


namespace maolan::audio
{
class Connectable
{
public:
  Connectable(const std::size_t &channels = 1);

  static nlohmann::json json();

  virtual nlohmann::json connections() = 0;
  virtual void setup();
  virtual void init();
  virtual void fetch();
  virtual void process();
  void connect(audio::IO *to);
  void connect(audio::IO *to, std::size_t inCh, std::size_t outCh);

protected:
  static std::vector<Connectable *> _all;

  std::vector<audio::Input *> _inputs;
  nlohmann::json conns(std::string_view name);
};
} // namespace maolan::audio
