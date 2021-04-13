#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <sys/soundcard.h>
#include <unistd.h>

#include "maolan/audio/oss/base.hpp"
#include "maolan/config.hpp"
#include "maolan/constants.hpp"


using namespace maolan::audio;


std::vector<OSSConfig *> OSS::_devices;


static void checkError(const int &value, const std::string &message)
{
  if (value == -1)
  {
    throw std::invalid_argument(message);
  }
}


/* Calculate frag by giving it minimal size of buffer */
static int size2frag(int x)
{
  int frag = 0;
  while ((1 << frag) < x)
  {
    ++frag;
  }
  return frag;
}


OSS::OSS(const std::string &deviceName, const int &argFrag,
         const int &sampleSize)
    : IO(deviceName, true, 0), _device{nullptr}
{

  bool found = false;
  for (const auto &iter : _devices)
  {
    if (iter->name == "OSS" && iter->device == deviceName)
    {
      found = true;
      _device = (OSSConfig *)iter;
      ++(_device->count);
      break;
    }
  }

  if (!found)
  {
    int error = 0;
    int tmp;
    _device = new OSSConfig;
    _device->frag = argFrag;
    _device->device = deviceName;
    _device->sampleSize = sampleSize;
    if (sampleSize == 4)
    {
      _device->format = AFMT_S32_NE;
    }
    else if (sampleSize == 2)
    {
      _device->format = AFMT_S16_NE;
    }
    else if (sampleSize == 1)
    {
      _device->format = AFMT_S8;
    }
    else
    {
      std::cerr << "Unsupported sample size: " << sampleSize << '\n';
      exit(1);
    }
    try
    {
      error = open(deviceName.data(), O_RDWR, 0);
      checkError(error, "open");
      _device->fd = error;

      _device->audioInfo.dev = -1;
      ioctl(_device->fd, SNDCTL_ENGINEINFO, &(_device->audioInfo));
      _outputs.resize(_device->audioInfo.max_channels);

      error =
          ioctl(_device->fd, SNDCTL_DSP_GETCAPS, &(_device->audioInfo.caps));
      checkError(error, "SNDCTL_DSP_GETCAPS");
      if (!(_device->audioInfo.caps & PCM_CAP_DUPLEX))
      {
        fprintf(stderr, "Device doesn't support full duplex!\n");
        exit(1);
      }

      tmp = channels();
      error = ioctl(_device->fd, SNDCTL_DSP_CHANNELS, &tmp);
      checkError(error, "SNDCTL_DSP_CHANNELS");

      tmp = _device->format;
      error = ioctl(_device->fd, SNDCTL_DSP_SETFMT, &tmp);
      checkError(error, "SNDCTL_DSP_SETFMT");
      if (tmp != _device->format)
      {
        std::stringstream s;
        s << _device << " doesn't support chosen sample format (";
        s << tmp << ")";
        error = 0;
        checkError(1, s.str());
      }

      tmp = Config::samplerate;
      error = ioctl(_device->fd, SNDCTL_DSP_SPEED, &tmp);
      checkError(error, "SNDCTL_DSP_SPEED");

      int minFrag = size2frag(_device->sampleSize * channels());
      if (_device->frag < minFrag)
      {
        _device->frag = minFrag;
      }
      tmp = _device->frag;
      error = ioctl(_device->fd, SNDCTL_DSP_SETFRAGMENT, &tmp);
      checkError(error, "SNDCTL_DSP_SETFRAGMENT");

      error = ioctl(_device->fd, SNDCTL_DSP_GETOSPACE, &(_device->bufferInfo));
      checkError(error, "SNDCTL_DSP_GETOSPACE");
    }
    catch (const std::invalid_argument &ex)
    {
      std::cerr << _type << " error: " << ex.what();
      std::cerr << ' ' << strerror(errno) << '\n';
      exit(1);
    }

    _device->sampleCount = _device->bufferInfo.bytes / _device->sampleSize;
    Config::audioBufferSize = _device->sampleCount / channels();
    _bytes = new int8_t[_device->bufferInfo.bytes];
    _devices.emplace(_devices.begin(), _device);
  }
}


nlohmann::json OSS::json()
{
  auto data = IO::json();
  data["bits"] = _device->sampleSize * 8;
  data["samplerate"] = _device->samplerate;
  return data;
}


OSS::~OSS()
{
  --(_device->count);
  if (_device->count < 1)
  {
    close(_device->fd);
    _devices.erase(std::find(_devices.begin(), _devices.end(), _device));
  }
}


size_t OSS::channels() const { return _outputs.size(); }
