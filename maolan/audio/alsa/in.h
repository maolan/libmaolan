#pragma once
#include <maolan/audio/alsa/base.h>
#include <string>


namespace maolan::audio
{
class ALSAIn : public ALSA
{
public:
  ALSAIn(const std::string &device, const int &channels,
         const snd_pcm_uframes_t &frames = 512);

  void fetch();
  void process();
};
} // namespace maolan::audio
