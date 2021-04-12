#pragma once
#include <string>
#include "maolan/audio/alsa/base.hpp"


namespace maolan::audio
{
class ALSAIn : public ALSA
{
public:
  ALSAIn(const std::string &device, const size_t &channels,
         const snd_pcm_uframes_t &frames = 512);

  void fetch();
  void process();
};
} // namespace maolan::audio
