#include "maolan/alsa/audio/out.hpp"
#include "maolan/audio/input.hpp"
#include "maolan/constants.hpp"


using namespace maolan::audio;


template <typename T>
ALSAOut<T>::ALSAOut(const std::string &name, const std::string &device)
  : ALSA{name, device, SND_PCM_STREAM_PLAYBACK, sizeof(T)}
{
  _type = "AudioALSAOut";
}


template <typename T> void ALSAOut<T>::fetch()
{
  ALSA::fetch();
  for (size_t i = 0; i < ALSA::channels(); ++i)
  {
    _outputs[i] = _inputs[i]->pull();
  }
}


template <typename T> void ALSAOut<T>::process()
{
  T *samples = (T *)_bytes;
  auto chs = ALSA::channels();
  for (std::size_t channel = 0; channel < chs; ++channel)
  {
    auto buffer = ALSA::_outputs[channel];
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


template <class T> void ALSAOut<T>::writehw()
{
  snd_pcm_writei(_handle, _bytes, _frames);
}


namespace maolan::audio
{
template class ALSAOut<int32_t>;
template class ALSAOut<int16_t>;
template class ALSAOut<int8_t>;
} // namespace maolan::audio
