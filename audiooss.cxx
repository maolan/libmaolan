#include <iostream>
#include <cstring>
#include <vector>
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
  outputThread = thread(&AudioOSSEngine::consume, this);
  inputThread = thread(&AudioOSSEngine::produce, this);
}

AudioOSSEngine::~AudioOSSEngine()
{
  quit = true;
  inputThread.join();
  outputThread.join();
}


void AudioOSSEngine::push(const int channel, const int sample)
{
  outputMutex.lock();
  waitOutputForSpace();
  output[channel].push_back(sample);
  outputMutex.unlock();
}


int AudioOSSEngine::pop(const int channel)
{
  inputMutex.lock();
  waitInputEmpty();
  auto &arr = input[channel];
  int sample = arr.front();
  arr.pop_front();
  inputMutex.unlock();
  return sample;
}


void AudioOSSEngine::produce()
{
  do
  {
    if (quit) {return;}
    inputMutex.lock();
    waitInputForSpace();
    // cout << "Producing sample " << position << endl;
    inputMutex.unlock();
  }
  while (true);
}


void AudioOSSEngine::consume()
{
  auto trig = PCM_ENABLE_INPUT | PCM_ENABLE_OUTPUT;
  int bytesWrote;
  do
  {
    if (quit) {return;}
    outputMutex.lock();
    auto &channel0 = output[0];
    auto &channel1 = output[1];
    vector<int> buffer;
    if (channel0.empty()) {cout << "No left channel" << endl;}
    if (channel1.empty()) {cout << "No right channel" << endl;}
    while (!channel0.empty() && !channel1.empty())
    {
      buffer.push_back(channel0.front());
      channel0.pop_front();
      buffer.push_back(channel1.front());
      channel1.pop_front();
    }
    outputMutex.unlock();
    bytesWrote = write(fd, buffer.data(), buffer.size());
    ioctl(fd, SNDCTL_DSP_SETTRIGGER, &trig);
    ++position;
  }
  while (bytesWrote != -1);
}


void AudioOSSEngine::sync()
{

  ioctl(fd, SNDCTL_DSP_SYNC, NULL);
}
