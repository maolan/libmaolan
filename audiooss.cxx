#include <iostream>
#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "audiooss.h"


AudioOSSEngine::AudioOSSEngine()
{
  quit = false;
  device = "/dev/dsp";
  channels = 2;
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
  inputThread = thread(&AudioOSSEngine::produce, this);
  outputThread = thread(&AudioOSSEngine::consume, this);
}

AudioOSSEngine::~AudioOSSEngine()
{
  quit = true;
  inputThread.join();
  outputThread.join();
}


void AudioOSSEngine::push(const Sample sample)
{
  outputMutex.lock();
  waitOutputForSpace();
  output.push_back(sample);
  outputMutex.unlock();
}


Sample & AudioOSSEngine::pop()
{
  inputMutex.lock();
  waitInputEmpty();
  Sample &sample = input.front();
  input.pop_front();
  inputMutex.unlock();
  return sample;
}


void AudioOSSEngine::produce()
{
  while (true)
  {
    if (quit) {return;}
    inputMutex.lock();
    waitInputForSpace();
    cout << "Producing" << endl;
    inputMutex.unlock();
  }
}


void AudioOSSEngine::consume()
{
  while (true)
  {
    if (quit) {return;}
    ++position;
    outputMutex.lock();
    cout << "Consuming" << endl;
    outputMutex.unlock();
  }
}
