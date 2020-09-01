#include <maolan/config.h>
#include <maolan/midi/io.h>
#include <vector>


using namespace maolan::midi;


std::vector<maolan::Config *> IO::devices;


IO::IO(const std::string &name, const bool &front, const bool &reg)
    : maolan::IO(name, front, reg)
{}


Buffer IO::pull(const std::size_t &ch)
{
  if (ch < outputs.size())
  {
    return outputs[ch];
  }
  return nullptr;
}


std::size_t IO::channels() const
{
  return outputs.size();
}
