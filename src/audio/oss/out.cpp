#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/soundcard.h>
#include <unistd.h>
#include "maolan/audio/oss/out.hpp"
#include "maolan/constants.hpp"


using namespace maolan::audio;


template <class T>
OSSOut<T>::OSSOut(const std::string &device, const int &frag)
    : OSS(device, frag, sizeof(T))
    , Connectable(channels())
{
  _type = "AudioOSSOut";
  _name = "OSS Audio Out";
}


template <class T>
void OSSOut<T>::fetch()
{
  Connectable::fetch();
  for (size_t i = 0; i < channels(); ++i)
  {
    outputs[i] = inputs[i].pull();
  }
}


template <class T>
void OSSOut<T>::convertToRaw()
{
  T *samples = (T *)bytes;
  auto chs = channels();
  for (auto channel = 0; channel < chs; ++channel)
  {
    auto buffer = outputs[channel];
    if (buffer == nullptr)
    {
      for (auto i = 0; i < device->audioBufferSize; ++i)
      {
        samples[i * chs + channel] = 0;
      }
    }
    else
    {
      for (auto i = 0; i < device->audioBufferSize; ++i)
      {
        auto &sample = buffer->data()[i];
        if (sample <= -1.0) { sample = -1.0; }
        else if (sample >= 1.0) { sample = 1.0; }
        samples[i * chs + channel] = sample * floatMaxInt;
      }
    }
  }
}


template <class T>
void OSSOut<T>::process()
{
  Connectable::process();
  convertToRaw();
  write(device->fd, bytes, device->bufferInfo.bytes);
}


namespace maolan::audio
{
template class OSSOut<int32_t>;
template class OSSOut<int16_t>;
template class OSSOut<int8_t>;
}
