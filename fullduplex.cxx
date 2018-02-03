#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/soundcard.h>
#include <iostream>


using namespace std;


string dspname = "/dev/dsp";
int fd_out = -1, fd_in = -1;
int dataSize = 64 * 1024;
int *rawData = new int[dataSize];		/* Max 32k local buffer */
int rate = 48000;
int fragsize;
int channels = 2;
int frag = 4;
int format = AFMT_S32_NE;


static void openInput(const string &dspname)
{
  oss_audioinfo ai;
  int tmp;
  int devcaps;

  if ((fd_in = open(dspname.data(), O_RDONLY, 0)) == -1)
  {
    cerr << dspname;
    cerr << strerror(errno) << endl;
    exit(1);
  }

  ai.dev = -1;
  if (ioctl(fd_in, SNDCTL_ENGINEINFO, &ai) != -1)
  {
    cout << "Using audio engine " << ai.dev;
    cout << " = " << ai.name << " for output" << endl;
  }

  if (ioctl(fd_in, SNDCTL_DSP_GETCAPS, &devcaps) == -1)
  {
    cerr << "SNDCTL_DSP_GETCAPS";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  tmp = frag;
  if (ioctl(fd_in, SNDCTL_DSP_SETFRAGMENT, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SETFRAGMENT";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  tmp = channels;
  if (ioctl(fd_in, SNDCTL_DSP_CHANNELS, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_CHANNELS";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  if (tmp != channels)
  {
    cerr << dspname << " doesn't support stereo (";
    cerr << tmp << ")" << endl;
    exit(1);
  }

  tmp = format;
  if (ioctl(fd_in, SNDCTL_DSP_SETFMT, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SETFMT";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  if (tmp == AFMT_S32_OE)
  {
    cerr << "Warning: Using 32 bit sample format with wrong endianess." << endl;
  }

  tmp = rate;
  if (ioctl(fd_in, SNDCTL_DSP_SPEED, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SPEED";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  if (tmp != rate)
  {
    cerr << dspname << " doesn't support requested rate ";
    cerr << rate << " (" << tmp << ")" << endl;
    exit(1);
  }
}


static void openOutput(const string &dspname)
{
  oss_audioinfo ai;
  int tmp;
  int devcaps;

  if ((fd_out = open(dspname.data(), O_WRONLY, 0)) == -1)
  {
    cerr << dspname;
    cerr << strerror(errno) << endl;
    exit(1);
  }
  ai.dev = -1;
  if (ioctl(fd_out, SNDCTL_ENGINEINFO, &ai) != -1)
  {
    cout << "Using audio engine " << ai.dev;
    cout << " = " << ai.name << " for output" << endl;
  }

  if (ioctl(fd_out, SNDCTL_DSP_GETCAPS, &devcaps) == -1)
  {
    cerr << "SNDCTL_DSP_GETCAPS";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  tmp = frag;
  if (ioctl(fd_out, SNDCTL_DSP_SETFRAGMENT, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SETFRAGMENT";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  tmp = channels;
  if (ioctl(fd_out, SNDCTL_DSP_CHANNELS, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_CHANNELS";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  if (tmp != channels)
  {
    cerr << dspname << " doesn't support stereo (";
    cerr << tmp << ")" << endl;
    exit(1);
  }

  tmp = format;
  if (ioctl(fd_out, SNDCTL_DSP_SETFMT, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SETFMT";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  if (tmp != AFMT_S32_NE && tmp != AFMT_S32_OE)
  {
    cerr << dspname << " doesn't support 32 bit sample format (";
    cerr << tmp << ")" << endl;
    exit(1);
  }

  tmp = rate;
  if (ioctl(fd_out, SNDCTL_DSP_SPEED, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SPEED";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  if (ioctl(fd_in, SNDCTL_DSP_GETBLKSIZE, &fragsize) == -1)
  {
    cerr << "SNDCTL_DSP_GETBLKSIZE";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  if (fragsize > dataSize)
  {
    cerr << "Too large fragment size " << fragsize << endl;
    exit(1);
  }
  cout << "Sample parameters set OK. Using fragment size " << fragsize << endl;
}


static void play(int fd_out, int fd_in)
{
  int l;
  int delay;
  float t;
  while ((l = read(fd_in, rawData, fragsize)) == fragsize)
  {
    if (ioctl(fd_out, SNDCTL_DSP_GETODELAY, &delay) == -1)
    {
      delay = 0;
    }
    delay /= 8;  /* Get number of 32 bit stereo samples */
    t = (float)delay / (float)rate;  /* Get delay in seconds */
    t *= 1000.0;  /* Convert delay to milliseconds */

    if ((l = write(fd_out, rawData, fragsize)) != fragsize)
    {
      cerr << "write";
      cerr << strerror(errno) << endl;
      exit(1);
    }
    cout << "\rDelay = " << t << "ms" << flush;
  }
  cerr << strerror(errno);
  exit(1);
}


int main (int argc, char **argv)
{

  if (argc > 1)
  {
    dspname = argv[1];
  }
  if (argc > 2)
  {
    rate = atoi (argv[3]);
    if (rate == 0)
    {
      rate = 48000;
    }
  }
  openInput(dspname);
  openOutput(dspname);
  play(fd_out, fd_in);
  delete []rawData;
  return 0;
}
