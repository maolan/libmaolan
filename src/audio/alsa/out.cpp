#include "maolan/audio/alsa/out.hpp"
#include "maolan/constants.hpp"
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/soundcard.h>
#include <unistd.h>


using namespace maolan::audio;


ALSAOut::ALSAOut(const std::string &device, const size_t &chs,
                 const snd_pcm_uframes_t &frames)
    : ALSA(device, chs), Connectable(chs)
{
  _type = "AudioALSAOut";
  _name = device;
  _outputs.resize(chs);
}


void ALSAOut::fetch()
{
  for (size_t i = 0; i < channels(); ++i)
  {
    _outputs[i] = _inputs[i]->pull();
  }
}


void ALSAOut::convertToRaw()
{
  auto chs = channels();
  for (std::size_t channel = 0; channel < chs; ++channel)
  {
    auto buffer = _outputs[channel];
    if (buffer == nullptr)
    {
      for (size_t i = 0; i < _device->audioBufferSize; ++i)
      {
        _frame[i * chs + channel] = 0;
      }
    }
    else
    {
      for (std::size_t i = 0; i < _device->audioBufferSize; ++i)
      {
        float sample = buffer->data()[i];

        if (sample <= -1.0)
        {
          sample = -1.0;
        }
        else if (sample >= 1.0)
        {
          sample = 1.0;
        }
        _frame[i * chs + channel] = sample * floatMaxInt;
      }
    }
  }
}


void ALSAOut::process()
{
  convertToRaw();
  play(_device->fragSize);
}


void ALSAOut::play(std::size_t dataSize)
{
  snd_pcm_writei(_device->handle, _frame, dataSize);
}
