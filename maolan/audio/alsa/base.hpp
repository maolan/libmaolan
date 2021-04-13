#pragma once
#include "maolan/audio/alsa/config.hpp"
#include "maolan/audio/io.hpp"
#include "maolan/constants.hpp"
#include <string>


namespace maolan::audio
{
class ALSA : public IO
{
public:
  ALSA(const std::string &device, const size_t &channels = 2,
       const snd_pcm_uframes_t &frames = 512);
  ~ALSA();

  virtual std::size_t channels() const;

protected:
  float *frame;
  ALSAConfig *device;
};
} // namespace maolan::audio
