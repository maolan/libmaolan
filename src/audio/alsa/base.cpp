#include <iostream>
#include "maolan/audio/alsa/base.hpp"
#include "maolan/audio/alsa/config.hpp"
#include "maolan/config.hpp"
#include "maolan/constants.hpp"


static void checkError(int &value, const std::string &message)
{
  if (value != 0)
  {
    throw std::invalid_argument(message);
  }
}


using namespace maolan::audio;


ALSA::ALSA(const std::string &_deviceName, const size_t &chs,
           const snd_pcm_uframes_t &frames)
    : IO(_deviceName, true, chs), _device{nullptr}
{
  bool found = false;
  for (const auto iter : _devices)
  {
    if (iter == _device)
    {
      found = true;
      _device = (ALSAConfig *)iter;
      ++(_device->count);
    }
  }

  int error = 0;
  if (!found)
  {
    _device = new ALSAConfig;
    _device->frames = frames;
    _device->audioChannels = chs;
    error = snd_pcm_open(&(_device->handle), _deviceName.data(), SND_PCM_STREAM_PLAYBACK, 0);
    checkError(error, "snd_pcm_open");
    if (_device->handle == nullptr)
    {
      return;
    }
    // Allocate and init a hardware parameters object
    snd_pcm_hw_params_alloca(&(_device->params));
    try
    {
      error = snd_pcm_hw_params_any(_device->handle, _device->params);
      checkError(error, "params");
      error = snd_pcm_hw_params_set_access(_device->handle, _device->params,
                                           SND_PCM_ACCESS_RW_INTERLEAVED);
      checkError(error, "access");
      error = snd_pcm_hw_params_set_format(_device->handle, _device->params,
                                           SND_PCM_FORMAT_FLOAT);
      checkError(error, "format");
      error = snd_pcm_hw_params_set_channels(_device->handle, _device->params,
                                             _device->audioChannels);
      checkError(error, "channels");
      error = snd_pcm_hw_params_set_rate_near(_device->handle, _device->params,
                                              &(Config::samplerate), 0);
      checkError(error, "sample rate");
      error = snd_pcm_hw_params_set_period_size_near(
          _device->handle, _device->params, &(_device->frames), 0);
      checkError(error, "period size");
      error = snd_pcm_hw_params(_device->handle, _device->params);
      checkError(error, "hw params");
    }
    catch (const std::invalid_argument &ex)
    {
      std::cerr << _type << " error: " << ex.what();
      if (error != 0)
      {
        std::cerr << ' ' << snd_strerror(error) << std::endl;
      }
      std::cerr << '\n';
      exit(1);
    }
    Config::audioBufferSize = _device->frames;
    std::cerr << Config::audioBufferSize << '\n';
  }
  _frame = new float[Config::audioBufferSize * sizeof(float)];
}


ALSA::~ALSA()
{
  delete[] _frame;
  --(_device->count);
  if (_device->count < 1)
  {
    for (auto iter = _devices.begin(); iter != _devices.end(); ++iter)
    {
      if (*iter == _device)
      {
        _devices.erase(iter);
      }
    }
  }
}


std::size_t ALSA::channels() const { return _outputs.size(); }
