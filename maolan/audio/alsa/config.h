#pragma once
#include <string>
#include <alsa/asoundlib.h>
#include <maolan/config.h>


namespace maolan::audio
{
class ALSAConfig : public Config
{
public:
  ALSAConfig();

  std::size_t audioChannels;
  int fragSize;
  std::string device;
  std::string dev;
	snd_pcm_uframes_t frames;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
};
} // namespace maolan::audio
