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


OSS::OSS(const std::string &deviceName, const int &argFrag, const int &chs)
    : IO(0, true, true, deviceName), device{nullptr}
{

  bool found = false;
  for (const auto iter : devices)
  {
    if (iter->name == "OSS" && iter->device == deviceName)
    {
      found = true;
      device = (OSSConfig *)iter;
      ++(device->count);
      outputs.resize(device->channels);
      break;
    }
  }

  if (!found)
  {
    int error = 0;
    int tmp;
    int devcaps;
    device = new OSSConfig;
    device->frag = argFrag;
    device->device = deviceName;
    try
    {
      error = open(deviceName.data(), O_RDWR, 0);
      checkError(error, "open");
      device->fd = error;

      device->audioInfo.dev = -1;
      ioctl(device->fd, SNDCTL_ENGINEINFO, &(device->audioInfo));

      error = ioctl(device->fd, SNDCTL_DSP_GETCAPS, &devcaps);
      checkError(error, "SNDCTL_DSP_GETCAPS");

      error = ioctl(device->fd, SNDCTL_DSP_SETFRAGMENT, &(device->frag));
      checkError(error, "SNDCTL_DSP_SETFRAGMENT");

      error = ioctl(device->fd, SNDCTL_DSP_GETBLKSIZE, &(device->fragSize));
      checkError(error, "SNDCTL_DSP_GETBLKSIZE");

      tmp = chs;
      error = ioctl(device->fd, SNDCTL_DSP_CHANNELS, &tmp);
      checkError(error, "SNDCTL_DSP_CHANNELS");
      outputs.resize(tmp);
      device->channels = tmp;

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

      tmp = device->samplerate;
      error = ioctl(device->fd, SNDCTL_DSP_SPEED, &tmp);
      checkError(error, "SNDCTL_DSP_SPEED");
    }
    catch (const std::invalid_argument &ex)
    {
      std::cerr << _type << " error: " << ex.what();
      std::cerr << ' ' << strerror(errno) << '\n';
      exit(1);
    }

    Config::audioBufferSize = device->fragSize / channels() / sizeof(int);
    devices.emplace(devices.begin(), device);
  }

  frame = new int[device->fragSize];
}


OSS::~OSS()
{
  delete[] frame;
  --(device->count);
  if (device->count < 1)
  {
    close(device->fd);
    devices.erase(std::find(devices.begin(), devices.end(), device));
  }
}


size_t OSS::channels() const { return outputs.size(); }
