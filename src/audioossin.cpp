#include <unistd.h>
#include <maolan/audioossin.h>
#include <maolan/constants.h>

using namespace maolan::audio;

AudioOSSIn::AudioOSSIn(const std::string &device, const size_t &chs)
  : AudioOSS(device)
{
  _name = "AudioOSSIn";
}


void AudioOSSIn::fetch()
{
  read(device->fd, rawData, device->fragSize);
}


void AudioOSSIn::process()
{
  auto chs = channels();
  int channel;
  int index;
  for (int i = 0; i < chs; ++i)
  {
    outputs[i] = AudioChunk(new AudioChunkData(device->audioChunkSize));
  }
  auto sizeLimit = device->fragSize / sizeof(int);
  for (int i = 0; i < sizeLimit; ++i)
  {
    channel = i % chs;
    index = i / chs;
    outputs[channel]->data[index] = rawData[i] / floatMaxInt;
  }
}
