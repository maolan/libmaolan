#include <iostream>
#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <maolan/audiooss>
#include <maolan/config>
#include <maolan/constants>


using namespace std;


AudioOSS::AudioOSS(const string &device)
{
  const int chs = 2;
  name = "AudioOSSOut";
  outputs.resize(chs);
  oss_audioinfo ai;
  int tmp;
  int devcaps;

  if ((fd = open(device.data(), O_RDWR, 0)) == -1)
  {
    cerr << device;
    cerr << strerror(errno) << endl;
    exit(1);
  }
  ai.dev = -1;
  if (ioctl(fd, SNDCTL_ENGINEINFO, &ai) != -1)
  {
    cout << "Using audio engine " << ai.dev;
    cout << " = " << ai.name << " for output" << endl;
  }

  if (ioctl(fd, SNDCTL_DSP_GETCAPS, &devcaps) == -1)
  {
    cerr << "SNDCTL_DSP_GETCAPS";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  tmp = Config::frag;
  if (ioctl(fd, SNDCTL_DSP_SETFRAGMENT, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SETFRAGMENT";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  if (ioctl(fd, SNDCTL_DSP_GETBLKSIZE, &Config::fragSize) == -1)
  {
    cerr << "SNDCTL_DSP_GETBLKSIZE: ";
    cerr << strerror(errno) << endl;
    exit(1);
  }


  tmp = channels();
  if (ioctl(fd, SNDCTL_DSP_CHANNELS, &tmp) == -1)
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

  tmp = Config::format;
  if (ioctl(fd, SNDCTL_DSP_SETFMT, &tmp) == -1)
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

  tmp = Config::samplerate;
  if (ioctl(fd, SNDCTL_DSP_SPEED, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SPEED";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  rawData = new int[Config::fragSize];
  cout << "Sample parameters for output set OK. Using fragment size " << Config::fragSize;
  cout << " and " << channels() << " channels." << endl;
  Config::audioChunkSize = Config::fragSize / channels() / sizeof(int);
}


AudioOSS::~AudioOSS()
{
  delete []rawData;
  close(fd);
}


size_t AudioOSS::channels() const
{
  return outputs.size();
}
