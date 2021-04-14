#include "maolan/audio/oss/out.hpp"
#include "maolan/constants.hpp"
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/soundcard.h>
#include <unistd.h>


using namespace maolan::audio;


template <typename T>
OSSOut<T>::OSSOut(const std::string &device, const int &frag)
    : OSS(device, frag, sizeof(T))
{
  std::cout << "OSSOut::OSSOut() " << _name << '\n';
  OSS::_type = "AudioOSSOut";
  OSS::_name = device;
}


template <typename T> void OSSOut<T>::fetch()
{
  for (size_t i = 0; i < OSS::channels(); ++i)
  {
    OSS::_outputs[i] = _inputs[i]->pull();
  }
}


template <typename T> void OSSOut<T>::convertToRaw()
{
  T *samples = (T *)_bytes;
  auto chs = OSS::channels();
  for (size_t channel = 0; channel < chs; ++channel)
  {
    auto buffer = OSS::_outputs[channel];
    if (buffer == nullptr)
    {
      for (size_t i = 0; i < _device->audioBufferSize; ++i)
      {
        samples[i * chs + channel] = 0;
      }
    }
    else
    {
      for (size_t i = 0; i < _device->audioBufferSize; ++i)
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
  std::cout << "Writing " << _device->bufferInfo.bytes << " bytes"
    << " to file descriptor " << _device->fd << '\n';
  write(_device->fd, _bytes, _device->bufferInfo.bytes);
}


namespace maolan::audio
{
template class OSSOut<int32_t>;
template class OSSOut<int16_t>;
template class OSSOut<int8_t>;
} // namespace maolan::audio
