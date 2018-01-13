#include <maolan/audioio>


using namespace std;


AudioIO::AudioIO(const size_t &size)
{
  if (size > 0)
  {
    outputs.resize(size);
  }
}


AudioChunk AudioIO::pull(const unsigned &channel)
{
  return outputs[channel];
}

