#include <iostream>
#include <deque>
#include <limits>
#include "audioio.h"


using namespace std;


AudioIO::AudioIO(const size_t &size)
{
  outputs.resize(size);
}


AudioChunk AudioIO::pull(const unsigned &channel)
{
  return outputs[channel];
}

