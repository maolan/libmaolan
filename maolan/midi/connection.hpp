#pragma once
#include "maolan/midi/io.hpp"


namespace maolan::midi
{
class Connection
{
public:
  Connection();
  Connection(midi::IO *connectTo, const std::size_t &ch);

  IO *get();
  void target(midi::IO *connectTo, const std::size_t &ch);
  Buffer pull();

protected:
  midi::IO *to;
  std::size_t channel;
};
} // namespace maolan::midi
