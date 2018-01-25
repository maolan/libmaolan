#include <iostream>
#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <maolan/audioossout>
#include <maolan/config>


using namespace std;


AudioOSSOut::AudioOSSOut(const size_t &size)
  : AudioIO(size)
{
  inputs.resize(size);
  string device = "/dev/dsp";
  size_t channels = size;
  format = AFMT_S32_NE;
  samplerate = 44100;
  if((fd = open(device.data(), O_WRONLY, 0)) == -1)
  {
    cerr << device << ' ' << strerror(errno) << endl;
    exit(1);
  }
  int tmp = format;
  if(ioctl(fd, SNDCTL_DSP_SETFMT, &tmp) == -1 || tmp != format)
  {
    cerr << "Setting audio format failed " << strerror(errno) << endl;
    exit(1);
  }
  tmp = channels;
  if(ioctl(fd, SNDCTL_DSP_CHANNELS, &tmp) == -1 || tmp != channels)
  {
    cerr << "Can not set number of channels to " << channels;
    cerr << ": " << strerror(errno) << endl;
    exit(1);
  }
  tmp = samplerate;
  if(ioctl(fd, SNDCTL_DSP_SPEED, &tmp) == -1 || tmp != samplerate)
  {
    cerr << "Can not set sampling frequency to " << samplerate << ": ";
    cerr << strerror(errno) << endl;
    exit(1);
  }
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


void AudioOSSOut::process()
{
  int result;
  float element;
  for (size_t i = 0; i < Config::audioChunkSize; ++i)
  {
    for (auto &channel : outputs)
    {
      if (channel == nullptr) {
        result = 0;
      }
      else
      {
        element = channel->data[i];
        if (element > 1.0)
        {
          result = numeric_limits<int>::max();
        }
        else if (element < -1.0)
        {
          result = numeric_limits<int>::min();
        }
        else
        {
          result = element * numeric_limits<int>::max();
        }
      }
      normalizedOut.push_back(result);
    }
  }
  int dataSize = normalizedOut.size() * sizeof(*normalizedOut.data());
  write(fd, normalizedOut.data(), dataSize);
  normalizedOut.clear();
}


size_t AudioOSSOut::channels() const
{
  return outputs.size();
}


void AudioOSSOut::connect(AudioIO *to)
{
  for (size_t channel = 0; channel < channels(); ++channel)
  {
    AudioConnection conn(to, channel);
    inputs[channel].add(conn);
  }
}


void AudioOSSOut::connect(AudioIO *to, std::size_t inCh, std::size_t outCh)
{
  AudioConnection conn(to, outCh);
  inputs[inCh].add(conn);
}
