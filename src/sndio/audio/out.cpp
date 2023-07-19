#include <iostream>
#include "maolan/sndio/audio/out.hpp"
#include "maolan/audio/input.hpp"
#include "maolan/constants.hpp"


using namespace maolan::audio;


template <typename T>
SNDIOOut<T>::SNDIOOut(const std::string &name, const std::string &device)
  : SNDIO{name, device, SIO_PLAY, sizeof(T)}
{
  _type = "AudioSNDIOOut";
}


template <typename T> void SNDIOOut<T>::fetch()
{
  SNDIO::fetch();
  for (size_t i = 0; i < SNDIO::channels(); ++i)
  {
    _outputs[i] = _inputs[i]->pull();
  }
}


template <typename T> void SNDIOOut<T>::process()
{
  T *samples = (T *)_bytes;
  auto chs = SNDIO::channels();
  for (std::size_t channel = 0; channel < chs; ++channel)
  {
    auto buffer = SNDIO::_outputs[channel];
    if (buffer == nullptr)
    {
      for (std::size_t i = 0; i < Config::audioBufferSize; ++i)
      {
        samples[i * chs + channel] = 0;
      }
    }
    else
    {
      for (size_t i = 0; i < Config::audioBufferSize; ++i)
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


template <class T> void SNDIOOut<T>::writehw()
{
  size_t size = Config::audioBufferSize * _parameters->rchan;
  sio_write(_handle, _bytes, size);
}


namespace maolan::audio
{
template class SNDIOOut<int32_t>;
template class SNDIOOut<int16_t>;
template class SNDIOOut<int8_t>;
} // namespace maolan::audio
