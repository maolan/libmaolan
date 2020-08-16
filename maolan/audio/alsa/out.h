#pragma once
#include <maolan/audio/alsa/base.h>
#include <maolan/audio/connectable.h>
#include <maolan/audio/input.h>
#include <string>


namespace maolan::audio
{
class ALSAOut : public ALSA, public Connectable
{
public:
  ALSAOut(const std::string &device, const int &channels,
          const snd_pcm_uframes_t &frames = 512);

  virtual void fetch();
  virtual void process();
  void convertToRaw();
  void play(float *frame, std::size_t dataSize);
};
} // namespace maolan::audio
