#include <iostream>
#include <cstring>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "fileinput.h"


using namespace std;


int main(int argc, char **argv)
{
  FileInput fileInput("/usr/home/meka/Files/reporter44k1.wav");
  int fd, format = AFMT_S32_NE;
  if((fd = open("/dev/dsp", O_WRONLY, 0)) == -1)
  {
    cerr << "/dev/dsp " << strerror(errno) << endl;
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
  while (true)
  {
    fileInput.fetch();
    const auto &data = fileInput.data();
    auto data_size = data.size() * sizeof(*data.data());
    write(fd, data.data(), data_size);
  }
  return 0;
}
