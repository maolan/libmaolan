#pragma once
#include <string>
#include <maolan/constants.h>
#include <maolan/audio/io.h>
#include <maolan/audio/alsa/config.h>


namespace maolan::audio
{
class ALSA : public IO
{
public:
  ALSA(const std::string &device, const int &channels = 2, const snd_pcm_uframes_t &frames = 512);
  ~ALSA();

  virtual std::size_t channels() const;

protected:
  float *frame;
  ALSAConfig *device;
};
} // namespace maolan::audio
