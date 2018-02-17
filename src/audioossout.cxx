#include <iostream>
#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <maolan/audioossout>
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
  for (auto channel = 0; channel < chs; ++channel)
  {
    auto buffer = outputs[channel];
    if (buffer == nullptr)
    {
      for (auto i = 0; i < it->audioChunkSize; ++i)
      {
        rawData[i * chs + channel] = 0;
      }
    }
    else
    {
      for (auto i = 0; i < it->audioChunkSize; ++i)
      {
        rawData[i * chs + channel] = buffer->data[i] * maxInt;
      }
    }
  }
}


void AudioOSSOut::process()
{
  convertToRaw();
  play(rawData, it->fragSize);
}


void AudioOSSOut::play(int *rawData, size_t dataSize)
{
  write(it->fd, rawData, dataSize);
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
