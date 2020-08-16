#pragma once
#include <maolan/midi/io.h>


namespace maolan::midi
{
class Connection
{
public:
  Connection();
  Connection(IO *connectTo);

  IO *get();
  void target(IO *connectTo);
  Buffer pull();

protected:
  IO *to;
};
} // namespace maolan::midi
