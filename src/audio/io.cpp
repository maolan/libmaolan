#include <maolan/audio/io.h>

using namespace maolan::audio;

IO::IO(const size_t &chs, bool front)
  : maolan::IO(front)
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
