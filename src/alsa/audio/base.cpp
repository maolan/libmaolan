#include <sstream>
#include <iostream>

#include "maolan/alsa/audio/base.hpp"


using namespace maolan::audio;


ALSA::ALSA(const std::string &name, const std::string &device, const snd_pcm_stream_t &stream, const size_t &sampleSize)
  : HW{name, device}
  , _sampleSize{sampleSize}
{
  int rc;
  int dir;
  size_t size;
  unsigned int val;

  // TODO: handle little endian
  if (_sampleSize == 4) { _format = SND_PCM_FORMAT_S32_BE; }
  else if (_sampleSize == 2) { _format = SND_PCM_FORMAT_S16_BE; }
  else if (_sampleSize == 1) { _format = SND_PCM_FORMAT_S8; }
  else
  {
    std::stringstream s;
    s << "Unsupported sample size: " << _sampleSize << '\n';
    throw std::invalid_argument(s.str());
  }

  /* Open PCM device for playback. */
  rc = snd_pcm_open(&_handle, _device.data(), SND_PCM_STREAM_PLAYBACK, 0);
  if (rc < 0)
  {
    fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
    exit(1);
  }

  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&_params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(_handle, _params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(_handle, _params, SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(_handle, _params, _format);

  snd_pcm_hw_params_get_channels(_params, &val);
  snd_pcm_hw_params_set_channels(_handle, _params, val);

  /* 48000 bits/second sampling rate */
  val = 48000;
  snd_pcm_hw_params_set_rate_near(_handle, _params, &val, &dir);

  /* Set period size to 32 _frames. */
  _frames = 32;
  snd_pcm_hw_params_set_period_size_near(_handle, _params, &_frames, &dir);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(_handle, _params);
  if (rc < 0)
  {
    fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
    exit(1);
  }

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(_params, &_frames, &dir);
  size = _frames * _sampleSize * 2;
  _bytes = new int8_t[size];
}
