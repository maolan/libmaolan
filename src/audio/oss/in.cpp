#include <unistd.h>
#include "maolan/audio/oss/in.hpp"
#include "maolan/constants.hpp"


using namespace maolan::audio;


OSSIn::OSSIn(const std::string &device, const int &frag)
    : OSS(device, frag)
{
  _type = "OSSIn";
  _name = "OSS In";
}


void OSSIn::fetch() { read(device->fd, bytes, device->bufferInfo.bytes); }


void OSSIn::process()
{
  auto chs = channels();
  int channel;
  int index;
  int32_t *samples = (int32_t *)bytes;
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
