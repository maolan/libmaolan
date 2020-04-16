#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/soundcard.h>
#include <unistd.h>
#include <maolan/config.h>
#include <maolan/constants.h>
#include <maolan/audio/oss/base.h>

using namespace maolan::audio;

std::vector<OSSConfig *> OSS::devices;


OSS::OSS(const std::string &deviceName, const int &argFrag, const int &chs)
  : IO(0, true, true, deviceName)
  , device{nullptr}
{

  bool found = false;
  for (auto iter = devices.begin(); iter < devices.end(); ++iter)
  {
    if (*iter == device)
    {
      found = true;
      device = *iter;
      ++(device->count);
    }
  }

  if (!found)
  {
    oss_audioinfo ai;
    int tmp;
    int devcaps;
    device = new OSSConfig;
    device->frag = argFrag;
    device->device = deviceName;
    if ((device->fd = open(deviceName.data(), O_RDWR, 0)) == -1)
    {
      std::cerr << deviceName << ": ";
      std::cerr << strerror(errno) << std::endl;
      exit(1);
    }
    ai.dev = -1;
    ioctl(device->fd, SNDCTL_ENGINEINFO, &ai);

    if (ioctl(device->fd, SNDCTL_DSP_GETCAPS, &devcaps) == -1)
    {
      std::cerr << "SNDCTL_DSP_GETCAPS";
      std::cerr << strerror(errno) << std::endl;
      exit(1);
    }

    tmp = device->frag;
    if (ioctl(device->fd, SNDCTL_DSP_SETFRAGMENT, &tmp) == -1)
    {
      std::cerr << "SNDCTL_DSP_SETFRAGMENT";
      std::cerr << strerror(errno) << std::endl;
      exit(1);
    }

    if (ioctl(device->fd, SNDCTL_DSP_GETBLKSIZE, &(device->fragSize)) == -1)
    {
      std::cerr << "SNDCTL_DSP_GETBLKSIZE: ";
      std::cerr << strerror(errno) << std::endl;
      exit(1);
    }

    tmp = chs;
    if (ioctl(device->fd, SNDCTL_DSP_CHANNELS, &tmp) == -1)
    {
      std::cerr << "SNDCTL_DSP_CHANNELS: ";
      std::cerr << strerror(errno) << std::endl;
      exit(1);
    }
    outputs.resize(tmp);

    tmp = device->format;
    if (ioctl(device->fd, SNDCTL_DSP_SETFMT, &tmp) == -1)
    {
      std::cerr << "SNDCTL_DSP_SETFMT";
      std::cerr << strerror(errno) << std::endl;
      exit(1);
    }
    if (tmp != AFMT_S32_NE && tmp != AFMT_S32_OE)
    {
      std::cerr << device << " doesn't support 32 bit sample format (";
      std::cerr << tmp << ")" << std::endl;
      exit(1);
    }

    tmp = device->samplerate;
    if (ioctl(device->fd, SNDCTL_DSP_SPEED, &tmp) == -1)
    {
      std::cerr << "SNDCTL_DSP_SPEED";
      std::cerr << strerror(errno) << std::endl;
      exit(1);
    }
    device->audioBufferSize = device->fragSize / channels() / sizeof(int);
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
