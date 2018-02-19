#include <iostream>
#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <maolan/audiooss>
#include <maolan/config>
#include <maolan/constants>


using namespace std;


vector<OSSConfig> AudioOSS::devices;


AudioOSS::AudioOSS(const string &device)
{
  const int chs = 2;
  outputs.resize(chs);

  bool found = false;
  for (auto iter = devices.begin(); iter < devices.end(); ++iter)
  {
    if (iter->device == device)
    {
      found = true;
      it = iter;
      ++it->count;
    }
  }

  if (!found)
  {
    oss_audioinfo ai;
    int tmp;
    int devcaps;
    OSSConfig config;
    config.device = device;
    if ((config.fd = open(device.data(), O_RDWR, 0)) == -1)
    {
      cerr << device;
      cerr << strerror(errno) << endl;
      exit(1);
    }
    ai.dev = -1;
    if (ioctl(config.fd, SNDCTL_ENGINEINFO, &ai) != -1)
    {
      cout << "Using OSS audio engine " << ai.dev;
      cout << " = " << ai.name << endl;
    }

    if (ioctl(config.fd, SNDCTL_DSP_GETCAPS, &devcaps) == -1)
    {
      cerr << "SNDCTL_DSP_GETCAPS";
      cerr << strerror(errno) << endl;
      exit(1);
    }

    tmp = config.frag;
    if (ioctl(config.fd, SNDCTL_DSP_SETFRAGMENT, &tmp) == -1)
    {
      cerr << "SNDCTL_DSP_SETFRAGMENT";
      cerr << strerror(errno) << endl;
      exit(1);
    }
    if (ioctl(config.fd, SNDCTL_DSP_GETBLKSIZE, &config.fragSize) == -1)
    {
      cerr << "SNDCTL_DSP_GETBLKSIZE: ";
      cerr << strerror(errno) << endl;
      exit(1);
    }

    tmp = channels();
    if (ioctl(config.fd, SNDCTL_DSP_CHANNELS, &tmp) == -1)
    {
      cerr << "SNDCTL_DSP_CHANNELS: ";
      cerr << strerror(errno) << endl;
      exit(1);
    }
    if (tmp != channels())
    {
      cerr << device << " doesn't support ";
      cout << channels() << " channels, but ";
      cerr << tmp << " instead" << endl;
      exit(1);
    }

    tmp = config.format;
    if (ioctl(config.fd, SNDCTL_DSP_SETFMT, &tmp) == -1)
    {
      cerr << "SNDCTL_DSP_SETFMT";
      cerr << strerror(errno) << endl;
      exit(1);
    }
    if (tmp != AFMT_S32_NE && tmp != AFMT_S32_OE)
    {
      cerr << device << " doesn't support 32 bit sample format (";
      cerr << tmp << ")" << endl;
      exit(1);
    }

    tmp = config.samplerate;
    if (ioctl(config.fd, SNDCTL_DSP_SPEED, &tmp) == -1)
    {
      cerr << "SNDCTL_DSP_SPEED";
      cerr << strerror(errno) << endl;
      exit(1);
    }
    config.audioChunkSize = config.fragSize / channels() / sizeof(int);
    it = devices.emplace(devices.begin(), config);
  }

  rawData = new int[it->fragSize];
}


AudioOSS::~AudioOSS()
{
  delete []rawData;
  --it->count;
  if (it->count < 1)
  {
    close(it->fd);
    devices.erase(it);
  }
}


size_t AudioOSS::channels() const
{
  return outputs.size();
}
