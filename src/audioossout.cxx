#include <iostream>
#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <maolan/audioossout>


using namespace std;


AudioOSSOut::AudioOSSOut(const size_t &size)
  : AudioIO(size)
{
  string device = "/dev/dsp";
  size_t channels = size;
  format = AFMT_S32_NE;
  samplerate = 44100;
  if((fd = open(device.data(), O_RDWR, 0)) == -1)
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
