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


template <class T>
void OSSIn<T>::fetch() { read(device->fd, bytes, device->bufferInfo.bytes); }


template <class T>
void OSSIn<T>::process()
{
  const auto &chs = channels();
  int channel;
  int index;
  T *samples = (T *)bytes;
  for (int i = 0; i < chs; ++i)
  {
    outputs[i] = std::make_shared<BufferData>(Config::audioBufferSize);
  }
  for (int i = 0; i < Config::audioBufferSize; ++i)
  {
    channel = i % chs;
    index = i / chs;
    outputs[channel]->data()[index] = samples[i] / floatMaxInt;
  }
}


namespace maolan::audio
{
template class OSSIn<int32_t>;
template class OSSIn<int16_t>;
template class OSSIn<int8_t>;
}
