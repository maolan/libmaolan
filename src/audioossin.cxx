#include <unistd.h>
#include <maolan/audioossin>
#include <maolan/constants>


using namespace std;


AudioOSSIn::AudioOSSIn(const string &device, const size_t &chs)
  : AudioOSS(device)
{
  name = "AudioOSSIn";
}


void AudioOSSIn::fetch()
{
  read(it->fd, rawData, it->fragSize);
}


void AudioOSSIn::process()
{
  auto chs = channels();
  int channel;
  int index;
  for (int i = 0; i < chs; ++i)
  {
    outputs[i] = AudioChunk(new AudioChunkData(it->audioChunkSize));
  }
  auto sizeLimit = it->fragSize / sizeof(int);
  for (int i = 0; i < sizeLimit; ++i)
  {
    channel = i % chs;
    index = i / chs;
    outputs[channel]->data[index] = rawData[i] / floatMaxInt;
  }
}
