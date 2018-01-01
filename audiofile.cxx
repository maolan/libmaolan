#include <iostream>
#include <cstring>
#include <sndfile.hh>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "audiofile.h"


const string device = "/dev/dsp";


AudioFile::AudioFile(const string &name)
  : name{name}
{}


void AudioFile::play(const string &file) const
{
  SndfileHandle audioFile{file.data()};
  const size_t bufferSize = audioFile.channels() * 128;
  size_t bytesRead;
  int buffer[bufferSize];
  int fd, format = AFMT_S32_NE;
  cout << "file: " << file << endl;
  cout << "samplerate: " << audioFile.samplerate() << endl;
  cout << "channels: " << audioFile.channels() << endl;
  cout << "frames: " << audioFile.frames() << endl;
  cout << "format: " << audioFile.format() << endl;

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
  tmp = audioFile.channels();
  if(ioctl(fd, SNDCTL_DSP_CHANNELS, &tmp) == -1 || tmp != audioFile.channels())
  {
    cerr << "Can not set number of channels to " << audioFile.channels();
    cerr << ": " << strerror(errno) << endl;
    exit(1);
  }
  tmp = audioFile.samplerate();
  if(ioctl(fd, SNDCTL_DSP_SPEED, &tmp) == -1 || tmp != audioFile.samplerate())
  {
    cerr << "Can not set sampling frequency to " << audioFile.samplerate() << ": ";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  while ((bytesRead = audioFile.read(buffer, bufferSize)) != 0)
  {
    write(fd, buffer, sizeof(buffer));
  }
  close(fd);
}
