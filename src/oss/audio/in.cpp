#include <unistd.h>
#include <poll.h>

#include "maolan/oss/audio/in.hpp"
#include "maolan/constants.hpp"


using namespace maolan::audio;


template <class T>
OSSIn<T>::OSSIn(const std::string &device, const int &frag)
    : OSS(device, frag, sizeof(T))
{
  _type = "AudioOSSIn";
  _name = device;
}


template <class T> void OSSIn<T>::readhw()
{
  read(_fd, _bytes, _bufferInfo.bytes);
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


template <class T> struct pollfd OSSIn<T>::pollfd()
{
  struct pollfd pfd;
  pfd.fd = _fd;
  pfd.events = POLLIN;
  return pfd;
}


namespace maolan::audio
{
template class OSSIn<int32_t>;
template class OSSIn<int16_t>;
template class OSSIn<int8_t>;
} // namespace maolan::audio
