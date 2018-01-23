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
open_one_device (char *dspname)
{
  oss_audioinfo ai;
  int fd;
  int tmp;
  int devcaps;
  int channels = 2;
  int format;
  int frag;

  if ((fd = open (dspname, O_RDWR, 0)) == -1)
  {
    perror (dspname);
    exit (-1);
  }

  ai.dev = -1;
  if (ioctl (fd, SNDCTL_ENGINEINFO, &ai) != -1)
  {
    printf ("\nUsing audio engine %d=%s for duplex\n\n", ai.dev, ai.name);
  }

#ifdef USE_RAW_FORMATS
  tmp = 0;
  ioctl (fd, SNDCTL_DSP_COOKEDMODE, &tmp);
#endif
  if (ioctl (fd, SNDCTL_DSP_GETCAPS, &devcaps) == -1)
  {
    perror ("SNDCTL_DSP_GETCAPS");
    exit (-1);
  }

  if (!(devcaps & PCM_CAP_DUPLEX))
  {
    fprintf (stderr,
       "%s doesn't support one device based full duplex scheme\n",
       dspname);
    fprintf (stderr, "Please use the two device scheme.\n");
    exit (-1);
  }
  if (ioctl (fd, SNDCTL_DSP_SETDUPLEX, NULL) == -1)
  {
    perror ("SNDCTL_DSP_SETDUPLEX");
    exit (-1);
  }
  frag = 0x7fff000a;		/* Unlimited number of 1k fragments */

  if (ioctl (fd, SNDCTL_DSP_SETFRAGMENT, &frag) == -1)
  {
    perror ("SNDCTL_DSP_SETFRAGMENT");
    exit (-1);
  }
  tmp = channels;

  if (ioctl (fd, SNDCTL_DSP_CHANNELS, &tmp) == -1)
  {
    perror ("SNDCTL_DSP_CHANNELS");
    exit (-1);
  }

  if (tmp != channels)
  {
    fprintf (stderr, "%s doesn't support stereo (%d)\n", dspname, tmp);
    exit (-1);
  }

  tmp = AFMT_S16_NE;

  if (ioctl (fd, SNDCTL_DSP_SETFMT, &tmp) == -1)
  {
    perror ("SNDCTL_DSP_SETFMT");
    exit (-1);
  }

  if (tmp != AFMT_S16_NE && tmp != AFMT_S16_OE)
  {
    fprintf (stderr, "%s doesn't support 16 bit sample format (%x)\n",
       dspname, tmp);
    exit (-1);
  }

  format = tmp;

  if (format == AFMT_S16_OE)
  {
    fprintf (stderr,
       "Warning: Using 16 bit sample format with wrong endianess.\n");
  }

  tmp = rate;

  if (ioctl (fd, SNDCTL_DSP_SPEED, &tmp) == -1)
  {
    perror ("SNDCTL_DSP_SPEED");
    exit (-1);
  }

  if (tmp != rate)
  {
    fprintf (stderr, "%s doesn't support requested rate %d (%d)\n", dspname,
       rate, tmp);
    exit (-1);
  }

  if (ioctl (fd, SNDCTL_DSP_GETBLKSIZE, &fragsize) == -1)
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
  fd_in = fd_out = fd;
}

static void
open_two_devices (char *dspname_out, char *dspname_in)
{
  oss_audioinfo ai_in, ai_out;
  int tmp;
  int devcaps;
  int channels = 2;
  int format;
  int frag = 0x7fff000a;	/* Unlimited number of 1k fragments */

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

#ifdef USE_RAW_FORMATS
  tmp = 0;
  ioctl (fd_out, SNDCTL_DSP_COOKEDMODE, &tmp);
  tmp = 0;
  ioctl (fd_in, SNDCTL_DSP_COOKEDMODE, &tmp);
#endif
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
#if 0
    exit (-1);
#endif
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
#if 0
    exit (-1);
#endif
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

static void
method_1 (int fd_out, int fd_in)
{

  char silence[32 * 1024];	/* Buffer for one fragment of silence */
  fd_set reads;
  int n;
  unsigned int trig;
  int first_time = 1;

  trig = 0;			/* Trigger OFF */
  if (ioctl (fd_in, SNDCTL_DSP_SETTRIGGER, &trig) == -1) perror ("SETTRIGGER 0");

  trig = PCM_ENABLE_INPUT;	/* Trigger ON */
  if (fd_in == fd_out) trig |= PCM_ENABLE_OUTPUT;
  if (ioctl (fd_in, SNDCTL_DSP_SETTRIGGER, &trig) == -1) perror ("SETTRIGGER 1");

  while (1)			/* Infinite loop */
  {
      struct timeval time;

      FD_ZERO (&reads);

      FD_SET (0, &reads);	/* stdin */
      FD_SET (fd_in, &reads);

      time.tv_sec = 1;
      time.tv_usec = 0;
      if ((n = select (fd_in + 1, &reads, NULL, NULL, &time)) == -1)
      {
        perror ("select");
        exit (-1);
      }

      if (n == 0)
      {
        fprintf (stderr, "Timeout\n");
        continue;
      }

      if (FD_ISSET (0, &reads))	/* Keyboard input */
      {
        printf ("Finished.\n");
        exit (0);
      }

      if (FD_ISSET (fd_in, &reads))
      {
        int l, n;
        struct audio_buf_info info;

        if (ioctl (fd_in, SNDCTL_DSP_GETISPACE, &info) == -1)
        {
          perror ("GETISPACE");
          exit (-1);
        }

        n = info.bytes;	/* How much */

        if ((l = read (fd_in, buffer, n)) == n)
        {
          printf ("\r %5d bytes ", n);
          fflush (stdout);

          if (first_time)
          {
            memset (silence, 0, fragsize);
            if (write (fd_out, silence, fragsize) != fragsize)
            {
              perror ("write");
              exit (-1);
            }
            first_time = 0;
          }
          if (write (fd_out, buffer, l) != l)
          {
            perror ("write");
            exit (-1);
          }
          continue;
        }
    }

    perror ("read");
    exit (-1);
  }
}

int
main (int argc, char *argv[])
{

  int method = 0;

  if (argc > 1)
    method = atoi (argv[1]);
  if (argc > 2)
  {
    rate = atoi (argv[2]);
    if (rate == 0) rate = 48000;
  }
  if (argc > 3) dspname = argv[3];
  if (argc > 4) dspname_in = argv[4];

  if (dspname_in == NULL) open_one_device (dspname);
  else open_two_devices (dspname, dspname_in);

  switch (method)
    {
    case 0:
      method_0 (fd_out, fd_in);
      break;

    case 1:
      method_1 (fd_out, fd_in);
      break;

    default:
      fprintf (stderr, "Method %d not defined\n", method);
      exit (-1);
    }

  exit (0);
}
