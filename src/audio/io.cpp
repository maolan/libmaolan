#include <maolan/audio/io.h>


using namespace maolan::audio;


IO::IO(const size_t &chs, const bool &front, const bool &reg)
  : maolan::IO("", front, reg)
{
  if (chs > 0)
  {
    outputs.resize(chs);
  }
}


IO::~IO()
{}


Chunk IO::pull(const unsigned &channel)
{
  return outputs[channel];
}


void IO::connect(IO *to)
{
  for (auto channel = 0; channel < channels(); ++channel)
  {
    connect(to, channel, channel);
  }
}


void IO::connect(IO *to, std::size_t inCh, std::size_t outCh)
{
}
