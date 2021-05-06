#include <unistd.h>
#include "maolan/audio/alsa/in.hpp"
#include "maolan/constants.hpp"

using namespace maolan::audio;

ALSAIn::ALSAIn(const std::string &device, const size_t &chs,
               const snd_pcm_uframes_t &frames)
    : ALSA(device, chs)
{
  _type = "AudioALSAIn";
  _name = device;
}


void ALSAIn::fetch()
{
  auto err = snd_pcm_readi(_device->handle, _frame, _device->fragSize);
}


void ALSAIn::process()
{
  auto chs = channels();
  size_t channel;
  int index;
  for (size_t i = 0; i < chs; ++i)
  {
    _outputs[i] = std::make_shared<BufferData>(Config::audioBufferSize);
  }
  auto sizeLimit = _device->fragSize / sizeof(*_frame);
  for (size_t i = 0; i < sizeLimit; ++i)
  {
    channel = i % chs;
    index = i / chs;
    _outputs[channel]->data()[index] = _frame[i] / floatMaxInt;
  }
}
