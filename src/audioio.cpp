#include <maolan/audioio.h>


AudioIO::AudioIO(const size_t &chs, bool front)
  : IO(front)
{
  if (chs > 0)
  {
    outputs.resize(chs);
  }
}


AudioIO::~AudioIO()
{}


AudioChunk AudioIO::pull(const unsigned &channel)
{
  return outputs[channel];
}
