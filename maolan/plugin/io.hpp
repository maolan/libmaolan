#pragma once
#include <nlohmann/json.hpp>

#include "maolan/audio/buffer.hpp"
#include "maolan/audio/connectable.hpp"
#include "maolan/audio/input.hpp"
#include "maolan/audio/io.hpp"
#include "maolan/midi/buffer.hpp"
#include "maolan/midi/connectable.hpp"
#include "maolan/midi/input.hpp"
#include "maolan/midi/io.hpp"


namespace maolan::plugin
{
class IO : public maolan::midi::IO,
           public maolan::audio::IO,
           public midi::Connectable,
           public audio::Connectable
{
public:
  IO(const std::string &name = "", const bool &reg = false);

  virtual std::size_t ports(const std::string &type = "audio",
                            const std::string &direction = "out") = 0;
  virtual void fetch();
  void connect(midi::IO *to);
  void connect(midi::IO *to, std::size_t inCh, std::size_t outCh);
  void connect(audio::IO *to);
  void connect(audio::IO *to, std::size_t inCh, std::size_t outCh);

  virtual audio::Buffer audio(const std::size_t &channel);
  virtual midi::Buffer midi(const std::size_t &channel);
  virtual nlohmann::json connections();
};
} // namespace maolan::plugin
