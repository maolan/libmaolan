#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/soundcard.h>
#include <unistd.h>

#include "maolan/constants.hpp"
#include "maolan/oss/audio/out.hpp"

using namespace maolan::audio;


template <typename T>
OSSOut<T>::OSSOut(const std::string &device, const int &frag)
    : OSS(device, frag, sizeof(T))
{
  _type = "AudioOSSOut";
}


template <typename T> void OSSOut<T>::fetch()
{
  OSS::fetch();
  for (size_t i = 0; i < OSS::channels(); ++i)
  {
    OSS::_outputs[i] = _inputs[i]->pull();
  }
}


template <typename T> void OSSOut<T>::convertToRaw()
{
  T *samples = (T *)_bytes;
  auto chs = OSS::channels();
  for (std::size_t channel = 0; channel < chs; ++channel)
  {
    auto buffer = OSS::_outputs[channel];
    if (buffer == nullptr)
    {
      for (std::size_t i = 0; i < Config::audioBufferSize; ++i)
      {
        samples[i * chs + channel] = 0;
      }
    }
    else
    {
      for (std::size_t i = 0; i < Config::audioBufferSize; ++i)
      {
        auto &sample = buffer->data()[i];
        if (sample <= -1.0)
        {
          sample = -1.0;
        }
        else if (sample >= 1.0)
        {
          sample = 1.0;
        }
        samples[i * chs + channel] = sample * floatMaxInt;
      }
    }
  }
}


template <typename T> void OSSOut<T>::process()
{
  convertToRaw();
  write(_fd, _bytes, _bufferInfo.bytes);
}


namespace maolan::audio
{
template class OSSOut<int32_t>;
template class OSSOut<int16_t>;
template class OSSOut<int8_t>;
} // namespace maolan::audio
