#include "maolan/constants.hpp"
#include "maolan/alsa/audio/in.hpp"


using namespace maolan::audio;


template <class T>
ALSAIn<T>::ALSAIn(const std::string &name, const std::string &device)
    : ALSA{name, device, SND_PCM_STREAM_CAPTURE, sizeof(T)}
{
  _type = "AudioALSAIn";
}


template <class T> void ALSAIn<T>::readhw()
{
  snd_pcm_readi(_handle, _bytes, _frames);
}


template <class T> void ALSAIn<T>::process()
{
  const auto &chs = channels();
  size_t channel;
  size_t index;
  T *samples = (T *)_bytes;
  for (size_t i = 0; i < chs; ++i)
  {
    _outputs[i] = std::make_shared<BufferData>(Config::audioBufferSize);
  }
  for (size_t i = 0; i < Config::audioBufferSize; ++i)
  {
    channel = i % chs;
    index = i / chs;
    _outputs[channel]->data()[index] = samples[i] / floatMaxInt;
  }
}


namespace maolan::audio
{
template class ALSAIn<int32_t>;
template class ALSAIn<int16_t>;
template class ALSAIn<int8_t>;
} // namespace maolan::audio
