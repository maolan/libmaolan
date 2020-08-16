#include <maolan/audio/oss/in.h>
#include <maolan/constants.h>
#include <unistd.h>

using namespace maolan::audio;

OSSIn::OSSIn(const std::string &device, const int &chs, const int &frag)
    : OSS(device, frag, chs)
{
  _type = "OSSIn";
  _name = "OSS In";
}


void OSSIn::fetch() { read(device->fd, frame, device->fragSize); }


void OSSIn::process()
{
  auto chs = channels();
  int channel;
  int index;
  for (int i = 0; i < chs; ++i)
  {
    outputs[i] = std::make_shared<BufferData>(Config::audioBufferSize);
  }
  auto sizeLimit = device->fragSize / sizeof(*frame);
  for (int i = 0; i < sizeLimit; ++i)
  {
    channel = i % chs;
    index = i / chs;
    outputs[channel]->data()[index] = frame[i] / floatMaxInt;
  }
}
