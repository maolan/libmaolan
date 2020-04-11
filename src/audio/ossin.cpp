#include <maolan/audio/ossin.h>
#include <maolan/constants.h>
#include <unistd.h>

using namespace maolan::audio;

OSSIn::OSSIn(const std::string &device, const std::size_t &chs, const std::size_t &frag)
  : OSS(device, frag)
{
  _type = "OSSIn";
}


void OSSIn::fetch() { read(device->fd, frame, device->fragSize); }


void OSSIn::process()
{
  auto chs = channels();
  int channel;
  int index;
  for (int i = 0; i < chs; ++i)
  {
    outputs[i] = Buffer(new BufferData(device->audioBufferSize));
  }
  auto sizeLimit = device->fragSize / sizeof(int);
  for (int i = 0; i < sizeLimit; ++i)
  {
    channel = i % chs;
    index = i / chs;
    outputs[channel]->data[index] = frame[i] / floatMaxInt;
  }
}
