#include <iostream>
#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <maolan/audioossin>
#include <maolan/config>
#include <maolan/constants>


using namespace std;


AudioOSSIn::AudioOSSIn(const size_t &chs)
  : AudioIO(chs)
{
  name = "AudioOSSIn";
  string device = "/dev/dsp";
  oss_audioinfo ai;
  int tmp;
  int devcaps;
  outputs.resize(chs);

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
    cout << " = " << ai.name << " for input" << endl;
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
  if (tmp == AFMT_S32_OE)
  {
    cerr << "Warning: Using 32 bit sample format with wrong endianess." << endl;
  }

  tmp = Config::samplerate;
  if (ioctl(fd, SNDCTL_DSP_SPEED, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SPEED";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  if (tmp != Config::samplerate)
  {
    cerr << device << " doesn't support requested samplerate ";
    cerr << Config::samplerate << " (" << tmp << ")" << endl;
    exit(1);
  }

  const auto rawDataSize = Config::fragSize / sizeof(int);
  Config::audioChunkSize = rawDataSize / channels();
  rawData = new int[rawDataSize];
  cout << "Sample parameters for input set OK. Using fragment size " << Config::fragSize << endl;
}


AudioOSSIn::~AudioOSSIn()
{
  close(fd);
  delete []rawData;
}


void AudioOSSIn::fetch()
{
  read(fd, rawData, Config::fragSize);
}


void AudioOSSIn::process()
{
  auto chs = channels();
  int channel;
  int index;
  for (int i = 0; i < chs; ++i)
  {
    outputs[i] = AudioChunk(new AudioChunkData(Config::audioChunkSize));
  }
  auto sizeLimit = Config::fragSize / sizeof(int);
  for (int i = 0; i < sizeLimit; ++i)
  {
    channel = i % chs;
    index = i / chs;
    outputs[channel]->data[index] = rawData[i] / floatMaxInt;
  }
}


size_t AudioOSSIn::channels() const
{
  return outputs.size();
}
