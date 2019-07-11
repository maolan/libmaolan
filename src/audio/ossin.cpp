#include <unistd.h>
#include <maolan/audio/ossin.h>
#include <maolan/constants.h>

using namespace maolan::audio;

OSSIn::OSSIn(const std::string &device, const size_t &chs)
  : OSS(device)
{
  _name = "OSSIn";
}


void OSSIn::fetch()
{
  read(device->fd, rawData, device->fragSize);
}


void OSSIn::process()
{
  auto chs = channels();
  int channel;
  int index;
  for (int i = 0; i < chs; ++i)
  {
    outputs[i] = Chunk(new ChunkData(device->audioChunkSize));
  }
  auto sizeLimit = device->fragSize / sizeof(int);
  for (int i = 0; i < sizeLimit; ++i)
  {
    channel = i % chs;
    index = i / chs;
    outputs[channel]->data[index] = rawData[i] / floatMaxInt;
  }
}
