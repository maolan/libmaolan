#pragma once
#include <maolan/audio/input.h>


namespace maolan::audio
{
class Connectable
{
public:
  Connectable(const std::size_t &channels);

  virtual void fetch();
  virtual void process();
  void connect(IO *to);
  void connect(IO *to, std::size_t inCh, std::size_t outCh);

protected:
  std::vector<Input> inputs;
};
} // namespace maolan::audio
