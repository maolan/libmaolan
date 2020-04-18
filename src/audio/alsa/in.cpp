#include <unistd.h>
#include <maolan/constants.h>
#include <maolan/audio/alsa/in.h>

using namespace maolan::audio;

ALSAIn::ALSAIn(const std::string &device, const int &chs, const snd_pcm_uframes_t &frames)
  : ALSA(device, chs)
{
  _type = "ALSAIn";
  _name = "ALSA In";
}


void ALSAIn::fetch()
{
  auto err = snd_pcm_readi(device->handle, frame, device->fragSize);
}


void ALSAIn::process()
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
