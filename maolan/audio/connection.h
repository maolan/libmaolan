#pragma once
#include <maolan/audio/io.h>


namespace maolan::audio
{
class Connection
{
public:
  Connection();
  Connection(IO *connectTo, const std::size_t &ch);

  IO *get();
  void target(IO *connectTo, const std::size_t &ch);
  Buffer pull();

protected:
  IO *to;
  std::size_t channel;
};
} // namespace maolan::audio
