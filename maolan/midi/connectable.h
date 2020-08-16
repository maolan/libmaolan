#pragma once
#include <maolan/midi/input.h>


namespace maolan::midi
{
class Connectable
{
public:
  virtual void fetch();
  virtual void process();
  void connect(IO *to);

protected:
  Input input;
};
} // namespace maolan::audio

