#include <maolan/audioio>


using namespace std;


vector<AudioIO *> AudioIO::ios;


AudioIO::AudioIO(const size_t &chs)
{
  name = "AudioIO";
  it = ios.emplace(ios.end(), this);
  if (chs > 0)
  {
    outputs.resize(chs);
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
