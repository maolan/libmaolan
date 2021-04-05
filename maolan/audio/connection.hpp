#pragma once
#include "maolan/audio/io.hpp"


namespace maolan::audio
{
class Connection
{
public:
  Connection();
  Connection(audio::IO *connectTo, const std::size_t &ch);

  IO *get();
  void target(audio::IO *connectTo, const std::size_t &ch);
  Buffer pull();
  std::size_t channel();
  void channel(const std::size_t &ch);

protected:
  audio::IO *to;
  std::size_t _channel;
};
} // namespace maolan::audio
