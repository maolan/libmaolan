#include <iostream>
#include <maolan/config.h>
#include <maolan/constants.h>
#include <maolan/audio/alsa/base.h>
#include <maolan/audio/alsa/config.h>


static void
checkError(int &value, const std::string &message)
{
  if (value != 0) {  throw std::invalid_argument(message); }
}


using namespace maolan::audio;



ALSA::ALSA(const std::string &deviceName, const int &chs, const snd_pcm_uframes_t &frames)
  : IO(0, true, true, deviceName)
  , device{nullptr}
{

  bool found = false;
  for (const auto iter : IO::devices)
  {
    if (iter == device)
    {
      found = true;
      device = (ALSAConfig *)iter;
      ++(device->count);
    }
  }

  int error = 0;
  if (!found)
  {
    device = new ALSAConfig;
    device->frames = frames;
    error = snd_pcm_open(&(device->handle), deviceName.data(), SND_PCM_STREAM_PLAYBACK, 0);
    checkError(error, "snd_pcm_open");
    if(device->handle == nullptr)
    {
      return;
    }
    // Allocate and init a hardware parameters object
    snd_pcm_hw_params_alloca(&(device->params));
    try
    {
      error = snd_pcm_hw_params_any(device->handle, device->params);
      checkError(error, "params");
      error = snd_pcm_hw_params_set_access(device->handle, device->params, SND_PCM_ACCESS_RW_INTERLEAVED);
      checkError(error, "access");
      error = snd_pcm_hw_params_set_format(device->handle, device->params, SND_PCM_FORMAT_FLOAT);
      checkError(error, "format");
      error = snd_pcm_hw_params_set_channels(device->handle, device->params, device->audioChannels);
      checkError(error, "channels");
      error = snd_pcm_hw_params_set_rate_near(device->handle, device->params, &(Config::samplerate), 0);
      checkError(error, "sample rate");
      error = snd_pcm_hw_params_set_period_size_near(device->handle, device->params, &(device->frames), 0);
      checkError(error, "period size");
      error = snd_pcm_hw_params(device->handle, device->params);
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
    Config::audioBufferSize = device->frames;
    std::cerr << Config::audioBufferSize << '\n';
  }
  frame = new float[Config::audioBufferSize * sizeof(float)];
}


ALSA::~ALSA()
{
  delete[] frame;
  --(device->count);
  if (device->count < 1)
  {
    for (auto iter = IO::devices.begin(); iter != IO::devices.end(); ++iter)
    {
      if (*iter == device) { devices.erase(iter); }
    }
  }
}


std::size_t ALSA::channels() const { return outputs.size(); }
