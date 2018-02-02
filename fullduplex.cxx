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
char buffer[32 * 1024];		/* Max 32k local buffer */
int rate = 48000;
int fragsize;


static void open_two_devices(const string &dspname_out, const string &dspname_in)
{
  oss_audioinfo ai_in, ai_out;
  int tmp;
  int devcaps;
  int channels = 2;
  int format;
  int frag = 6;

  if ((fd_out = open(dspname_out.data(), O_WRONLY, 0)) == -1)
  {
    cerr << dspname_out;
    cerr << strerror(errno) << endl;
    exit(1);
  }

  ai_out.dev = -1;
  if (ioctl (fd_out, SNDCTL_ENGINEINFO, &ai_out) != -1)
  {
    cout << "Using audio engine " << ai_out.dev;
    cout << " = " << ai_out.name << " for output" << endl;
  }
  if ((fd_in = open(dspname_in.data(), O_RDONLY, 0)) == -1)
  {
    cerr << dspname_in;
    cerr << strerror(errno) << endl;
    exit(1);
  }

  ai_in.dev = -1;
  if (ioctl (fd_in, SNDCTL_ENGINEINFO, &ai_in) != -1)
  {
    cout << "Using audio engine " << ai_in.dev;
    cout << " = " << ai_in.name << " for output" << endl;
  }

  if (ai_in.rate_source != ai_out.rate_source)
  {
    cerr << "Note! " << dspname_out << " and " << dspname_in << " are not";
    cerr << " necessarily driven by the same clock." << endl;
  }

  if (ioctl (fd_out, SNDCTL_DSP_GETCAPS, &devcaps) == -1)
  {
    cerr << "SNDCTL_DSP_GETCAPS";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  if (devcaps & PCM_CAP_DUPLEX)
  {
    cerr << "Device " << dspname_out << " supports duplex so you may want to";
    cerr << " use the single device approach instead." << endl;
  }

  if (ioctl (fd_in, SNDCTL_DSP_GETCAPS, &devcaps) == -1)
  {
    cerr << "SNDCTL_DSP_GETCAPS";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  tmp = frag;
  if (ioctl (fd_out, SNDCTL_DSP_SETFRAGMENT, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SETFRAGMENT";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  tmp = frag;
  if (ioctl (fd_in, SNDCTL_DSP_SETFRAGMENT, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SETFRAGMENT";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  tmp = channels;
  if (ioctl (fd_out, SNDCTL_DSP_CHANNELS, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_CHANNELS";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  if (tmp != channels)
  {
    cerr << dspname_out << " doesn't support stereo (";
    cerr << tmp << ")" << endl;
    exit(1);
  }
  if (ioctl (fd_in, SNDCTL_DSP_CHANNELS, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_CHANNELS";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  if (tmp != channels)
  {
    cerr << dspname_in << " doesn't support stereo (";
    cerr << tmp << ")" << endl;
    exit(1);
  }
  tmp = AFMT_S16_NE;

  if (ioctl (fd_out, SNDCTL_DSP_SETFMT, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SETFMT";
    cerr << strerror(errno) << endl;
    exit(1);
  }
  if (tmp != AFMT_S16_NE && tmp != AFMT_S16_OE)
  {
    cerr << dspname_out << " doesn't support 16 bit sample format (";
    cerr << tmp << ")" << endl;
    exit(1);
  }

  format = tmp;
  if (ioctl (fd_in, SNDCTL_DSP_SETFMT, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SETFMT";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  if (tmp != format)
  {
    cerr << "Error: Input and output devices use different formats ";
    cerr << "(" << tmp << "/" << format << ")" << endl;
    exit(1);
  }

  if (format == AFMT_S16_OE)
  {
    cerr << "Warning: Using 16 bit sample format with wrong endianess." << endl;
  }
  tmp = rate;

  if (ioctl (fd_in, SNDCTL_DSP_SPEED, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SPEED";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  if (tmp != rate)
  {
    cerr << dspname_out << " doesn't support requested rate ";
    cerr << rate << " (" << tmp << ")" << endl;
    exit(1);
  }

  if (ioctl (fd_out, SNDCTL_DSP_SPEED, &tmp) == -1)
  {
    cerr << "SNDCTL_DSP_SPEED";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  if (tmp != rate)
  {
    cerr << dspname_out << " doesn't support the same rate ";
    cerr << rate << "!=" << tmp << " as " << dspname_in << endl;
    exit(1);
  }
  if (ioctl (fd_in, SNDCTL_DSP_GETBLKSIZE, &fragsize) == -1)
  {
    cerr << "SNDCTL_DSP_GETBLKSIZE";
    cerr << strerror(errno) << endl;
    exit(1);
  }

  if (fragsize > sizeof (buffer))
  {
    cerr << "Too large fragment size " << fragsize << endl;
    exit(1);
  }
  cout << "Sample parameters set OK. Using fragment size " << fragsize << endl;
}


static void play(int fd_out, int fd_in)
{

  int l;
  int loopcount = 0;
  char silence[32 * 1024];	/* Buffer for one fragment of silence */
  while ((l = read (fd_in, buffer, fragsize)) == fragsize)
  {
    int delay;
    float t;

    if (loopcount == 0)
    {
      memset (silence, 0, fragsize);
      if (write (fd_out, silence, fragsize) != fragsize)
      {
        cerr << "write";
        cerr << strerror(errno) << endl;
        exit(1);
      }
    }
    if (ioctl (fd_out, SNDCTL_DSP_GETODELAY, &delay) == -1) delay = 0;
    delay /= 4;		/* Get number of 16 bit stereo samples */
    t = (float) delay / (float) rate;	/* Get delay in seconds */
    t *= 1000.0;		/* Convert delay to milliseconds */

    if ((l = write (fd_out, buffer, fragsize)) != fragsize)
    {
      cerr << "write";
      cerr << strerror(errno) << endl;
      exit(1);
    }

#if 1
    cout << "\rDelay = " << t << "ms" << flush;
#endif
    loopcount++;
  }
  cerr << strerror(errno);
  exit(1);
}


int
main (int argc, char *argv[])
{

  if (argc > 1) dspname = argv[1];
  if (argc > 2)
  {
    rate = atoi (argv[3]);
    if (rate == 0) rate = 48000;
  }
  open_two_devices(dspname, dspname);
  play(fd_out, fd_in);

  return 0;
}
