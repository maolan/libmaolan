#pragma once
#include <maolan/audio/input.h>
#include <vector>


namespace maolan::audio
{
class Connectable
{
public:
  Connectable(const std::size_t &channels = 1);

  virtual void fetch();
  virtual void process();
  void connect(audio::IO *to);
  void connect(audio::IO *to, std::size_t inCh, std::size_t outCh);

protected:
  std::vector<audio::Input> inputs;
};
} // namespace maolan::audio
