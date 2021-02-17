#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/soundcard.h>
#include <unistd.h>
#include "maolan/audio/oss/out.hpp"
#include "maolan/constants.hpp"

using namespace maolan::audio;

OSSOut::OSSOut(const std::string &device, const int &frag)
    : OSS(device, frag)
    , Connectable(channels())
{
  _type = "OSSOut";
}


void OSSOut::fetch()
{
  Connectable::fetch();
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
        float sample = buffer->data()[i];

        if (sample <= -1.0)
        {
          sample = -1.0;
        }
        else if (sample >= 1.0)
        {
          sample = 1.0;
        }
        frame[i * chs + channel] = sample * floatMaxInt;
      }
    }
  }
}


void OSSOut::process()
{
  Connectable::process();
  convertToRaw();
  play(frame, device->bufferInfo.bytes);
}


void OSSOut::play(int *frame, size_t dataSize)
{
  write(device->fd, frame, dataSize);
}
