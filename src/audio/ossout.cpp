#include <cstring>
#include <fcntl.h>
#include <maolan/audio/ossout.h>
#include <maolan/constants.h>
#include <sys/soundcard.h>
#include <unistd.h>

using namespace maolan::audio;

OSSOut::OSSOut(const std::string &device, const std::size_t &chs, const std::size_t &frag)
  : OSS(device, frag)
  , Connectable(chs)
{
  _type = "OSSOut";
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
      for (auto i = 0; i < device->audioBufferSize; ++i)
      {
        frame[i * chs + channel] = 0;
      }
    }
    else
    {
      for (auto i = 0; i < device->audioBufferSize; ++i)
      {
        float sample = buffer->data[i];
        if (sample <= -1.0)
        {
          sample = -1.0;
        }
        else if (sample >= 1.0)
        {
          sample = 1.0;
        }
        frame[i * chs + channel] = sample * maxInt;
      }
    }
  }
}


void OSSOut::process()
{
  convertToRaw();
  play(frame, device->fragSize);
}


void OSSOut::play(int *frame, size_t dataSize)
{
  write(device->fd, frame, dataSize);
}
