#pragma once
#include <maolan/audio/buffer.h>
#include <maolan/audio/connectable.h>
#include <maolan/audio/input.h>
#include <maolan/audio/io.h>
#include <maolan/midi/buffer.h>
#include <maolan/midi/connectable.h>
#include <maolan/midi/input.h>
#include <maolan/midi/io.h>


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
};
} // namespace maolan::plugin
