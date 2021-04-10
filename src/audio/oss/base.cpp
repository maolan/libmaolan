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
    : IO(deviceName, true, 0), device{nullptr}
{

  bool found = false;
  for (const auto &iter : devices)
  {
    if (iter->name == "OSS" && iter->device == deviceName)
    {
      found = true;
      device = (OSSConfig *)iter;
      ++(device->count);
      break;
    }
  }

  if (!found)
  {
    int error = 0;
    int tmp;
    device = new OSSConfig;
    device->frag = argFrag;
    device->device = deviceName;
    device->sampleSize = sampleSize;
    if (sampleSize == 4)
    {
      device->format = AFMT_S32_NE;
    }
    else if (sampleSize == 2)
    {
      device->format = AFMT_S16_NE;
    }
    else if (sampleSize == 1)
    {
      device->format = AFMT_S8;
    }
    else
    {
      std::cerr << "Unsupported sample size: " << sampleSize << '\n';
      exit(1);
    }
    this->sampleSize = sampleSize;
    try
    {
      error = open(deviceName.data(), O_RDWR, 0);
      checkError(error, "open");
      device->fd = error;

      device->audioInfo.dev = -1;
      ioctl(device->fd, SNDCTL_ENGINEINFO, &(device->audioInfo));
      _outputs.resize(device->audioInfo.max_channels);

      error = ioctl(device->fd, SNDCTL_DSP_GETCAPS, &(device->audioInfo.caps));
      checkError(error, "SNDCTL_DSP_GETCAPS");
      if (!(device->audioInfo.caps & PCM_CAP_DUPLEX))
      {
        fprintf(stderr, "Device doesn't support full duplex!\n");
        exit(1);
      }

      tmp = channels();
      error = ioctl(device->fd, SNDCTL_DSP_CHANNELS, &tmp);
      checkError(error, "SNDCTL_DSP_CHANNELS");

      tmp = device->format;
      error = ioctl(device->fd, SNDCTL_DSP_SETFMT, &tmp);
      checkError(error, "SNDCTL_DSP_SETFMT");
      if (tmp != device->format)
      {
        std::stringstream s;
        s << device << " doesn't support chosen sample format (";
        s << tmp << ")";
        error = 0;
        checkError(1, s.str());
      }

      tmp = Config::samplerate;
      error = ioctl(device->fd, SNDCTL_DSP_SPEED, &tmp);
      checkError(error, "SNDCTL_DSP_SPEED");

      int minFrag = size2frag(device->sampleSize * channels());
      if (device->frag < minFrag)
      {
        device->frag = minFrag;
      }
      tmp = device->frag;
      error = ioctl(device->fd, SNDCTL_DSP_SETFRAGMENT, &tmp);
      checkError(error, "SNDCTL_DSP_SETFRAGMENT");

      error = ioctl(device->fd, SNDCTL_DSP_GETOSPACE, &(device->bufferInfo));
      checkError(error, "SNDCTL_DSP_GETOSPACE");
    }
    catch (const std::invalid_argument &ex)
    {
      std::cerr << _type << " error: " << ex.what();
      std::cerr << ' ' << strerror(errno) << '\n';
      exit(1);
    }

    device->sampleCount = device->bufferInfo.bytes / device->sampleSize;
    Config::audioBufferSize = device->sampleCount / channels();
    bytes = new int8_t[device->bufferInfo.bytes];
    devices.emplace(devices.begin(), device);
  }
}


nlohmann::json OSS::json()
{
  auto data = IO::json();
  data["bits"] = sampleSize * 8;
  data["samplerate"] = device->samplerate;
  return data;
}


OSS::~OSS()
{
  --(device->count);
  if (device->count < 1)
  {
    close(device->fd);
    devices.erase(std::find(devices.begin(), devices.end(), device));
  }
}


size_t OSS::channels() const { return _outputs.size(); }
