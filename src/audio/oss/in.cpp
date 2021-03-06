#include <unistd.h>

#include "maolan/audio/oss/in.hpp"
#include "maolan/constants.hpp"


using namespace maolan::audio;


template <class T>
OSSIn<T>::OSSIn(const std::string &device, const int &frag)
    : OSS(device, frag, sizeof(T))
{
  _type = "AudioOSSIn";
  _name = device;
}


template <class T> void OSSIn<T>::fetch()
{
  read(_device->fd, _bytes, _device->bufferInfo.bytes);
}


template <class T> void OSSIn<T>::process()
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
template class OSSIn<int32_t>;
template class OSSIn<int16_t>;
template class OSSIn<int8_t>;
} // namespace maolan::audio
