#include <iostream>
#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <maolan/audioossout>
#include <maolan/config>
#include <maolan/constants>


using namespace std;


AudioOSSOut::AudioOSSOut(const string &device, const size_t &chs)
  : AudioOSS(device)
{
  name = "AudioOSSOut";
  inputs.resize(chs);
}


void AudioOSSOut::fetch()
{
  for (size_t i = 0; i < channels(); ++i)
  {
    outputs[i] = inputs[i].pull();
  }
}


void AudioOSSOut::convertToRaw()
{
  auto chs = channels();
  for (auto i = 0; i < Config::audioChunkSize; ++i)
  {
    auto inputIndex = i % chs;
    auto buffer = inputs[inputIndex].pull();
    if (buffer == nullptr) {rawData[i] = 0;}
    else
    {
      auto sample = buffer->data[i];
      if (sample <= -1.0)
      {
        rawData[i] = floatMinInt;
      }
      else if (sample >= 1.0)
      {
        rawData[i] = floatMaxInt;
      }
      else
      {
        rawData[i] = sample * floatMaxInt;
      }
    }
  }
}


void AudioOSSOut::process()
{
  convertToRaw();
  play(rawData, Config::fragSize);
}


void AudioOSSOut::play(int *rawData, size_t dataSize)
{
  write(fd, rawData, dataSize);
}


void AudioOSSOut::connect(AudioIO *to)
{
  for (auto channel = 0; channel < channels(); ++channel)
  {
    connect(to, channel, channel);
  }
}


void AudioOSSOut::connect(AudioIO *to, std::size_t inCh, std::size_t outCh)
{
  inputs[inCh].add(to, outCh);
}
