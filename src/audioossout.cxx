#include <iostream>
#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <maolan/audioossout>
#include <maolan/config>
#include <maolan/constants>


using namespace std;


AudioOSSOut::AudioOSSOut(const size_t &chs)
  : AudioIO(chs)
{
  name = "AudioOSSOut";
  inputs.resize(chs);
  outputs.resize(chs);
  string device = "/dev/dsp";
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
  cout << "Sample parameters for output set OK. Using fragment size " << Config::fragSize << endl;
}


AudioOSSOut::~AudioOSSOut()
{
  close(fd);
}


void AudioOSSOut::sync() const
{
  auto trig = PCM_ENABLE_INPUT | PCM_ENABLE_OUTPUT;
  ioctl(fd, SNDCTL_DSP_SETTRIGGER, &trig);
  ioctl(fd, SNDCTL_DSP_SYNC, NULL);
}


void AudioOSSOut::fetch()
{
  for (size_t i = 0; i < channels(); ++i)
  {
    outputs[i] = inputs[i].pull();
  }
}


void AudioOSSOut::convertToRaw()
{
  auto chs = channels();
  for (auto i = 0; i < Config::audioChunkSize; ++i)
  {
    auto inputIndex = i % chs;
    auto buffer = inputs[inputIndex].pull();
    if (buffer == nullptr) {rawData[i] = 0;}
    else
    {
      auto sample = buffer->data[i];
      if (sample <= -1.0)
      {
        rawData[i] = floatMinInt;
      }
      else if (sample >= 1.0)
      {
        rawData[i] = floatMaxInt;
      }
      else
      {
        rawData[i] = sample * floatMaxInt;
      }
    }
  }
}


void AudioOSSOut::process()
{
  convertToRaw();
  play(rawData, Config::fragSize);
}


void AudioOSSOut::play(int *rawData, size_t dataSize)
{
  write(fd, rawData, dataSize);
}


size_t AudioOSSOut::channels() const
{
  return outputs.size();
}


void AudioOSSOut::connect(AudioIO *to)
{
  for (auto channel = 0; channel < channels(); ++channel)
  {
    connect(to, channel, channel);
  }
}


void AudioOSSOut::connect(AudioIO *to, std::size_t inCh, std::size_t outCh)
{
  inputs[inCh].add(to, outCh);
}
