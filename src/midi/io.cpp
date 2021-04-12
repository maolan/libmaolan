#include "maolan/midi/io.hpp"
#include "maolan/config.hpp"
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


std::size_t IO::channels() const { return _outputs.size(); }
