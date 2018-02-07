#include <iostream>
#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <maolan/audioossin>
#include <maolan/config>


using namespace std;


AudioOSSIn::AudioOSSIn(const size_t &chs)
  : AudioIO(chs)
{
  string device = "/dev/dsp";
  int format = AFMT_S32_NE;
  oss_audioinfo ai;
  int tmp;
  int devcaps;
  samplerate = 48000;
  outputs.resize(chs);
  frag = 4;

  if ((fd = open(device.data(), O_RDONLY, 0)) == -1)
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

  tmp = frag;
  if (ioctl(fd, SNDCTL_DSP_SETFRAGMENT, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SETFRAGMENT";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  if (ioctl(fd, SNDCTL_DSP_GETBLKSIZE, &fragsize) == -1)
  {
    cerr << "SNDCTL_DSP_GETBLKSIZE";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  rawData = new int[fragsize];

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

  tmp = format;
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

  tmp = samplerate;
  if (ioctl(fd, SNDCTL_DSP_SPEED, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SPEED";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  if (tmp != samplerate)
  {
    cerr << device << " doesn't support requested samplerate ";
    cerr << samplerate << " (" << tmp << ")" << endl;
    exit(1);
  }

  cout << "Sample parameters for input set OK. Using fragment size " << fragsize << endl;
}


AudioOSSIn::~AudioOSSIn()
{
  close(fd);
  delete []rawData;
}


void AudioOSSIn::fetch()
{
  int dataSize = fragsize * sizeof(rawData[0]);
  read(fd, rawData, dataSize);
}


void AudioOSSIn::process()
{
  auto chs = channels();
  auto chunkSize = fragsize / chs;
  for (int i = 0; i < chs; ++i)
  {
    outputs[i] = AudioChunk(new AudioChunkData(chunkSize));
  }
  for (int i = 0; i < fragsize; ++i)
  {
    auto channel = i % chs;
    outputs[channel]->data[i / chs] = rawData[i];
  }
}


size_t AudioOSSIn::channels() const
{
  return outputs.size();
}
