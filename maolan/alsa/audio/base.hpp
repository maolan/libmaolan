#pragma once
#include <alsa/asoundlib.h>
#include <nlohmann/json.hpp>

#include "maolan/audio/hw.hpp"


namespace maolan::audio
{
class ALSA : public HW
{
public:
  ALSA(const std::string &name, const std::string &device, const snd_pcm_stream_t &stream, const size_t &sampleSize);
  ~ALSA();

  virtual size_t channels() const;
  virtual nlohmann::json json();

protected:
  snd_pcm_t *_handle;
  snd_pcm_hw_params_t *_params;
  snd_pcm_uframes_t _frames;
  snd_pcm_format_t _format;
  int8_t *_bytes;
  size_t _sampleSize;
};
}
