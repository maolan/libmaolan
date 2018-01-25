#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/soundcard.h>

char *dspname = "/dev/dsp";
char *dspname_in = NULL;

int fd_out = -1, fd_in = -1;
char buffer[32 * 1024];		/* Max 32k local buffer */
int rate = 48000;
int fragsize;


static void
open_two_devices (char *dspname_out, char *dspname_in)
{
  oss_audioinfo ai_in, ai_out;
  int tmp;
  int devcaps;
  int channels = 2;
  int format;
  int frag = 10;

  if ((fd_out = open (dspname_out, O_WRONLY, 0)) == -1)
  {
    perror (dspname_out);
    exit (-1);
  }

  ai_out.dev = -1;
  if (ioctl (fd_out, SNDCTL_ENGINEINFO, &ai_out) != -1)
  {
    printf ("\nUsing audio engine %d=%s for output\n", ai_out.dev,
      ai_out.name);
  }
  if ((fd_in = open (dspname_in, O_RDONLY, 0)) == -1)
  {
    perror (dspname_in);
    exit (-1);
  }

  ai_in.dev = -1;
  if (ioctl (fd_in, SNDCTL_ENGINEINFO, &ai_in) != -1)
  {
    printf ("Using audio engine %d=%s for input\n\n", ai_in.dev,
      ai_in.name);
  }

  if (ai_in.rate_source != ai_out.rate_source)
  {
    fprintf (stderr,
       "Note! %s and %s are not necessarily driven by the same clock.\n",
       dspname_out, dspname_in);
  }

  if (ioctl (fd_out, SNDCTL_DSP_GETCAPS, &devcaps) == -1)
  {
    perror ("SNDCTL_DSP_GETCAPS");
    exit (-1);
  }

  if (devcaps & PCM_CAP_DUPLEX)
  {
    fprintf (stderr,
       "Device %s supports duplex so you may want to use the single device approach instead\n",
       dspname_out);
  }

  if (!(devcaps & PCM_CAP_OUTPUT))
  {
    fprintf (stderr, "%s doesn't support output\n", dspname_out);
    fprintf (stderr, "Please use different device.\n");
    exit (-1);
  }
  if (ioctl (fd_in, SNDCTL_DSP_GETCAPS, &devcaps) == -1)
  {
    perror ("SNDCTL_DSP_GETCAPS");
    exit (-1);
  }

  if (devcaps & PCM_CAP_DUPLEX)
  {
    fprintf (stderr,
       "Device %s supports duplex so you may want to use the single device approach instead\n",
       dspname_in);
  }

  if (!(devcaps & PCM_CAP_INPUT))
  {
    fprintf (stderr, "%s doesn't support input\n", dspname_in);
    fprintf (stderr, "Please use different device.\n");
    exit (-1);
  }
  if (ioctl (fd_out, SNDCTL_DSP_SETFRAGMENT, &frag) == -1)
  {
    perror ("SNDCTL_DSP_SETFRAGMENT");
    exit (-1);
  }
  if (ioctl (fd_in, SNDCTL_DSP_SETFRAGMENT, &frag) == -1)
  {
    perror ("SNDCTL_DSP_SETFRAGMENT");
    exit (-1);
  }

  tmp = channels;

  if (ioctl (fd_out, SNDCTL_DSP_CHANNELS, &tmp) == -1)
  {
    perror ("SNDCTL_DSP_CHANNELS");
    exit (-1);
  }

  if (tmp != channels)
  {
    fprintf (stderr, "%s doesn't support stereo (%d)\n", dspname_out, tmp);
    exit (-1);
  }
  if (ioctl (fd_in, SNDCTL_DSP_CHANNELS, &tmp) == -1)
  {
    perror ("SNDCTL_DSP_CHANNELS");
    exit (-1);
  }
  if (tmp != channels)
  {
    fprintf (stderr, "%s doesn't support stereo (%d)\n", dspname_in, tmp);
    exit (-1);
  }
  tmp = AFMT_S16_NE;

  if (ioctl (fd_out, SNDCTL_DSP_SETFMT, &tmp) == -1)
  {
    perror ("SNDCTL_DSP_SETFMT");
    exit (-1);
  }
  if (tmp != AFMT_S16_NE && tmp != AFMT_S16_OE)
  {
    fprintf (stderr, "%s doesn't support 16 bit sample format (%x)\n",
       dspname_out, tmp);
    exit (-1);
  }

  format = tmp;
  if (ioctl (fd_in, SNDCTL_DSP_SETFMT, &tmp) == -1)
  {
    perror ("SNDCTL_DSP_SETFMT");
    exit (-1);
  }

  if (tmp != format)
  {
    fprintf (stderr,
       "Error: Input and output devices use different formats (%x/%x)\n",
       tmp, format);
    exit (-1);
  }

  if (format == AFMT_S16_OE)
  {
    fprintf (stderr,
       "Warning: Using 16 bit sample format with wrong endianess.\n");
  }
  tmp = rate;

  if (ioctl (fd_in, SNDCTL_DSP_SPEED, &tmp) == -1)
  {
    perror ("SNDCTL_DSP_SPEED");
    exit (-1);
  }

  if (tmp != rate)
  {
    fprintf (stderr, "%s doesn't support requested rate %d (%d)\n",
       dspname_out, rate, tmp);
    exit (-1);
  }

  if (ioctl (fd_out, SNDCTL_DSP_SPEED, &tmp) == -1)
  {
    perror ("SNDCTL_DSP_SPEED");
    exit (-1);
  }

  if (tmp != rate)
  {
    fprintf (stderr, "%s doesn't support the same rate %d!=%d as %s\n",
       dspname_out, rate, tmp, dspname_in);
    exit (-1);
  }
  if (ioctl (fd_in, SNDCTL_DSP_GETBLKSIZE, &fragsize) == -1)
  {
    perror ("SNDCTL_DSP_GETBLKSIZE");
    exit (-1);
  }

  if (fragsize > sizeof (buffer))
  {
    fprintf (stderr, "Too large fragment size %d\n", fragsize);
    exit (-1);
  }
  printf ("Sample parameters set OK. Using fragment size %d\n", fragsize);
}


static void
method_0 (int fd_out, int fd_in)
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
        perror ("write");
        exit (-1);
      }
    }
    if (ioctl (fd_out, SNDCTL_DSP_GETODELAY, &delay) == -1) delay = 0;
    delay /= 4;		/* Get number of 16 bit stereo samples */
    t = (float) delay / (float) rate;	/* Get delay in seconds */
    t *= 1000.0;		/* Convert delay to milliseconds */

    if ((l = write (fd_out, buffer, fragsize)) != fragsize)
    {
      perror ("write");
      exit (-1);
    }

#if 1
      printf ("\rDelay=%5.3g msec", t);
      fflush (stdout);
#endif

      loopcount++;
    }

  perror ("read");
  exit (-1);
}


int
main (int argc, char *argv[])
{

  if (argc > 1) dspname = argv[1];
  if (argc > 2) dspname_in = argv[2];
  if (argc > 3)
  {
    rate = atoi (argv[3]);
    if (rate == 0) rate = 48000;
  }


  if (dspname_in == NULL) open_two_devices (dspname, dspname);
  else open_two_devices (dspname, dspname_in);

  method_0(fd_out, fd_in);

  return 0;
}
