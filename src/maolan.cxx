#include <iostream>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <maolan/audiofileinput>


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
  int result;
  float element;
  for (size_t i = 0; i < AudioFileInput::size; ++i)
  {
    for (auto &channel : outputs)
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
      data.push_back(result);
    }
  }
  int dataSize = data.size() * sizeof(*data.data());
  write(fd, data.data(), dataSize);
  data.clear();
}


int runClasses(int argc, char **argv)
{
  if (argc < 2)
  {
    cerr << "Usage: " << argv[0] << " <wav file>" << endl;
    return 1;
  }
  AudioFileInput f(argv[1]);
  vector<AudioChunk> outputs;
  outputs.resize(f.channels());
  cout << "Playing ..." << flush;
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
  cout << endl;
  return 0;
}


int main(int argc, char **argv)
{
  return runClasses(argc, argv);
}
