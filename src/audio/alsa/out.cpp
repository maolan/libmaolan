#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/soundcard.h>
#include <maolan/constants.h>
#include <maolan/audio/alsa/out.h>

using namespace maolan::audio;

ALSAOut::ALSAOut(const std::string &device, const int &chs, const snd_pcm_uframes_t &frames)
  : ALSA(device, chs)
  , Connectable(chs)
{
  _type = "ALSAOut";
  outputs.resize(chs);
}


void ALSAOut::fetch()
{
  for (size_t i = 0; i < channels(); ++i) { outputs[i] = inputs[i].pull(); }
}


void ALSAOut::convertToRaw()
{
  auto chs = channels();
  for (auto channel = 0; channel < chs; ++channel)
  {
    auto buffer = outputs[channel];
    if (buffer == nullptr)
    {
      for (auto i = 0; i < device->audioBufferSize; ++i)
      {
        frame[i * chs + channel] = 0;
      }
    }
    else
    {
      for (auto i = 0; i < device->audioBufferSize; ++i)
      {
        float sample = buffer->data[i];

        if (sample <= -1.0) { sample = -1.0; }
        else if (sample >= 1.0) { sample = 1.0; }
        frame[i * chs + channel] = sample * maxInt;
      }
    }
  }
}


void ALSAOut::process()
{
  convertToRaw();
  play(frame, device->fragSize);
}


void ALSAOut::play(float *frame, std::size_t dataSize)
{
	snd_pcm_writei(device->handle, frame, dataSize);
}
