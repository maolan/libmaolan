#include "maolan/midi/io.hpp"
#include "maolan/config.hpp"
#include "maolan/midi/input.hpp"
#include <vector>


using namespace maolan::midi;


std::vector<maolan::Config *> IO::devices;


IO::IO(const std::string &name, const bool &reg) : maolan::IO(name, reg) {}


Buffer IO::pull(const std::size_t &ch)
{
  if (ch < _outputs.size())
  {
    return _outputs[ch];
  }
  return nullptr;
}


void IO::connect(IO *to)
{
  for (size_t channel = 0; channel < _inputs.size(); ++channel)
  {
    connect(to, channel, channel);
  }
}


void IO::connect(IO *to, std::size_t inCh, std::size_t outCh)
{
  _inputs[inCh]->connect(to, outCh);
}


std::size_t IO::channels() const { return _outputs.size(); }
