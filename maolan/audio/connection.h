#pragma once
#include <maolan/audio/io.h>


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

protected:
  audio::IO *to;
  std::size_t channel;
};
} // namespace maolan::audio
