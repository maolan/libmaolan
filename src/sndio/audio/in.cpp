#include "maolan/constants.hpp"
#include "maolan/sndio/audio/in.hpp"


using namespace maolan::audio;


template <class T>
SNDIOIn<T>::SNDIOIn(const std::string &name, const std::string &device)
    : SNDIO{name, device, SIO_REC, sizeof(T)}
{
  _type = "AudioSNDIOIn";
}


template <class T> void SNDIOIn<T>::readhw()
{
  size_t size = Config::audioBufferSize * _parameters->rchan;
  sio_read(_handle, _bytes, size);
}


template <class T> void SNDIOIn<T>::process()
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
template class SNDIOIn<int32_t>;
template class SNDIOIn<int16_t>;
template class SNDIOIn<int8_t>;
} // namespace maolan::audio
