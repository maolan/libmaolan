#include <iostream>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include "audiofileinput.h"


using namespace std;


int openAudioDevice(const string &path)
{
  int fd, format = AFMT_S32_NE;
  if((fd = open(path.data(), O_WRONLY, 0)) == -1)
  {
    cerr << path << ' ' << strerror(errno) << endl;
    exit(1);
  }
  int tmp = format;
  if(ioctl(fd, SNDCTL_DSP_SETFMT, &tmp) == -1 || tmp != format)
  {
    cerr << "Setting audio format failed " << strerror(errno) << endl;
    exit(1);
  }
  tmp = 2;
  if(ioctl(fd, SNDCTL_DSP_CHANNELS, &tmp) == -1 || tmp != 2)
  {
    cerr << "Can not set number of channels to " << 2;
    cerr << ": " << strerror(errno) << endl;
    exit(1);
  }
  tmp = 44100;
  if(ioctl(fd, SNDCTL_DSP_SPEED, &tmp) == -1 || tmp != 44100)
  {
    cerr << "Can not set sampling frequency to " << 44100 << ": ";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  return fd;
}


int fd = openAudioDevice("/dev/dsp");


void play(vector<AudioChunk> &outputs)
{
  vector<int> data;
  for (size_t i = 0; i < AudioFileInput::size; ++i)
  {
    for (auto &channel : outputs)
    {
      auto &element = channel->data[i];
      int result = element * numeric_limits<int>::max();
      data.push_back(result);
    }
  }
  int dataSize = data.size() * sizeof(*data.data());
  write(fd, data.data(), dataSize);
}


int runClasses(int argc, char **argv)
{
  AudioFileInput f(argv[1]);
  vector<AudioChunk> outputs;
  outputs.resize(f.channels());
  while (true)
  {
    // Fetch
    f.fetch();

    // Pull
    for (size_t channel = 0; channel < f.channels(); ++channel)
    {
      outputs[channel] = f.pull(channel);
    }

    // Process
    play(outputs);
  }
  return 0;
}


int main(int argc, char **argv)
{
  return runClasses(argc, argv);
}
