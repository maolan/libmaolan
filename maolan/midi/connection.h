#pragma once
#include <maolan/midi/io.h>


namespace maolan::midi
{
class Connection
{
public:
  Connection();
  Connection(MIDIIO *connectTo);

  MIDIIO *get();
  void target(MIDIIO *connectTo);
  Buffer pull();

protected:
  MIDIIO *to;
};
} // namespace maolan::midi
