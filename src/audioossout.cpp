#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <maolan/audioossout.h>
#include <maolan/constants.h>

using namespace maolan::audio;

AudioOSSOut::AudioOSSOut(const std::string &device, const size_t &chs)
  : AudioOSS(device)
{
  _name = "AudioOSSOut";
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
      for (auto i = 0; i < device->audioChunkSize; ++i)
      {
        rawData[i * chs + channel] = 0;
      }
    }
    else
    {
      for (auto i = 0; i < device->audioChunkSize; ++i)
      {
        float sample = buffer->data[i];
        if (sample <= -1.0) {sample = 1.0;}
        else if (sample >= 1.0) {sample = 1.0;}
        rawData[i * chs + channel] = sample * maxInt;
      }
    }
  }
}


void AudioOSSOut::process()
{
  convertToRaw();
  play(rawData, device->fragSize);
}


void AudioOSSOut::play(int *rawData, size_t dataSize)
{
  write(device->fd, rawData, dataSize);
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
