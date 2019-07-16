#include <cstring>
#include <fcntl.h>
#include <maolan/audio/ossout.h>
#include <maolan/constants.h>
#include <sys/soundcard.h>
#include <unistd.h>

using namespace maolan::audio;

OSSOut::OSSOut(const std::string &device, const size_t &chs) : OSS(device)
{
  _type = "OSSOut";
  inputs.resize(chs);
}


void OSSOut::fetch()
{
  for (size_t i = 0; i < channels(); ++i)
  {
    outputs[i] = inputs[i].pull();
  }
}


void OSSOut::convertToRaw()
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
        if (sample <= -1.0)
        {
          sample = 1.0;
        }
        else if (sample >= 1.0)
        {
          sample = 1.0;
        }
        rawData[i * chs + channel] = sample * maxInt;
      }
    }
  }
}


void OSSOut::process()
{
  convertToRaw();
  play(rawData, device->fragSize);
}


void OSSOut::play(int *rawData, size_t dataSize)
{
  write(device->fd, rawData, dataSize);
}


void OSSOut::connect(IO *to)
{
  for (auto channel = 0; channel < channels(); ++channel)
  {
    connect(to, channel, channel);
  }
}


void OSSOut::connect(IO *to, std::size_t inCh, std::size_t outCh)
{
  inputs[inCh].add(to, outCh);
}
