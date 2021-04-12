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
    : OSS(device, frag, sizeof(T)), Connectable(channels())
{
  _type = "AudioOSSOut";
  _name = device;
}


template <typename T> void OSSOut<T>::fetch()
{
  Connectable::fetch();
  for (size_t i = 0; i < channels(); ++i)
  {
    _outputs[i] = _inputs[i]->pull();
  }
}


template <typename T> void OSSOut<T>::convertToRaw()
{
  T *samples = (T *)bytes;
  auto chs = channels();
  for (std::size_t channel = 0; channel < chs; ++channel)
  {
    auto buffer = _outputs[channel];
    if (buffer == nullptr)
    {
      for (std::size_t i = 0; i < device->audioBufferSize; ++i)
      {
        samples[i * chs + channel] = 0;
      }
    }
    else
    {
      for (std::size_t i = 0; i < device->audioBufferSize; ++i)
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
  Connectable::process();
  convertToRaw();
  write(device->fd, bytes, device->bufferInfo.bytes);
}


template <typename T> nlohmann::json OSSOut<T>::connections()
{
  return conns(_name);
}


template <typename T> void OSSOut<T>::init() { Connectable::init(); }
template <typename T> void OSSOut<T>::setup() { Connectable::setup(); }


namespace maolan::audio
{
template class OSSOut<int32_t>;
template class OSSOut<int16_t>;
template class OSSOut<int8_t>;
} // namespace maolan::audio
