#include <maolan/audioio>


using namespace std;


vector<AudioIO *> AudioIO::ios;


AudioIO::AudioIO(const size_t &outs)
{
  it = ios.emplace(ios.end(), this);
  if (outs > 0)
  {
    outputs.resize(outs);
  }
}


AudioIO::~AudioIO()
{
  ios.erase(it);
}


AudioChunk AudioIO::pull(const unsigned &channel)
{
  return outputs[channel];
}


void AudioIO::process(){}
