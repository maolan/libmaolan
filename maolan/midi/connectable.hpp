#pragma once
#include <vector>
#include "maolan/midi/input.hpp"


namespace maolan::midi
{
class Connectable
{
public:
  Connectable(const std::size_t &channels = 1);

  virtual void fetch();
  virtual void process();
  void connect(midi::IO *to);
  void connect(midi::IO *to, std::size_t inCh, std::size_t outCh);

protected:
  std::vector<midi::Input> inputs;
};
} // namespace maolan::midi

