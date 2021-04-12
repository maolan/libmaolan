#pragma once
#include <string>
#include "maolan/audio/alsa/base.hpp"
#include "maolan/audio/connectable.hpp"
#include "maolan/audio/input.hpp"


namespace maolan::audio
{
class ALSAOut : public ALSA, public Connectable
{
public:
  ALSAOut(const std::string &device, const size_t &channels,
          const snd_pcm_uframes_t &frames = 512);

  virtual void fetch();
  virtual void process();
  void convertToRaw();
  void play(float *frame, std::size_t dataSize);
};
} // namespace maolan::audio
