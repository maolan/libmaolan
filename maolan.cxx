#include <iostream>
#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "fileinput.h"


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


int main(int argc, char **argv)
{
  FileInput fileInput("/usr/home/meka/Files/reporter44k1.wav");
  int fd = openAudioDevice("/dev/dsp");
  size_t dataSize = 1;
  vector<int> data;
  while (dataSize > 0)
  {
    fileInput.fetch();
    Chunk chunkL = fileInput.pull(0);
    Chunk chunkR = fileInput.pull(1);
    for (int i = 0; i < chunkL->size(); ++i)
    {
      data.push_back((*chunkL)[i] * numeric_limits<int>::max());
      data.push_back((*chunkR)[i] * numeric_limits<int>::max());
    }
    dataSize = data.size() * sizeof(*data.data());
    write(fd, data.data(), dataSize);
    data.clear();
  }
  return 0;
}
