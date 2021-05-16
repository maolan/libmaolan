#include <alsa/asoundlib.h>
#include <stdio.h>

// source: http://www.saunalahti.fi/~s7l/blog/2005/08/21/Full%20Duplex%20ALSA

int restarting;

int nchannels = 1;
int buffer_size = 512;
int sample_rate = 44100;
int bits = 16;

char *snd_device_in = "plughw:0,0";
char *snd_device_out = "plughw:0,0";
snd_pcm_t *playback_handle;
snd_pcm_t *capture_handle;

int configure_alsa_audio(snd_pcm_t *device, int channels)
{
  snd_pcm_hw_params_t *hw_params;
  int err;
  unsigned int tmp;
  snd_pcm_uframes_t frames;
  int fragments = 2;

  /* allocate memory for hardware parameter structure */
  if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0)
  {
    fprintf(stderr, "cannot allocate parameter structure (%s)\n",
            snd_strerror(err));
    return 1;
  }
  /* fill structure from current audio parameters */
  if ((err = snd_pcm_hw_params_any(device, hw_params)) < 0)
  {
    fprintf(stderr, "cannot initialize parameter structure (%s)\n",
            snd_strerror(err));
    return 1;
  }

  /* set access type, sample rate, sample format, channels */
  if ((err = snd_pcm_hw_params_set_access(device, hw_params,
                                          SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
  {
    fprintf(stderr, "cannot set access type: %s\n", snd_strerror(err));
    return 1;
  }
  // bits = 16
  if ((err = snd_pcm_hw_params_set_format(device, hw_params,
                                          SND_PCM_FORMAT_S16_LE)) < 0)
  {
    fprintf(stderr, "cannot set sample format: %s\n", snd_strerror(err));
    return 1;
  }
  tmp = sample_rate;
  if ((err = snd_pcm_hw_params_set_rate_near(device, hw_params, &tmp, 0)) < 0)
  {
    fprintf(stderr, "cannot set sample rate: %s\n", snd_strerror(err));
    return 1;
  }
  if (tmp != sample_rate)
  {
    fprintf(stderr,
            "Could not set requested sample rate, asked for %d got %d\n",
            sample_rate, tmp);
    sample_rate = tmp;
  }
  if ((err = snd_pcm_hw_params_set_channels(device, hw_params, channels)) < 0)
  {
    fprintf(stderr, "cannot set channel count: %s\n", snd_strerror(err));
    return 1;
  }

  int frame_size = channels * (bits / 8);
  frames = buffer_size / frame_size * fragments;
  if ((err = snd_pcm_hw_params_set_buffer_size_near(device, hw_params,
                                                    &frames)) < 0)
  {
    fprintf(stderr, "Error setting buffer_size %d frames: %s\n", frames,
            snd_strerror(err));
    return 1;
  }
  if (buffer_size != frames * frame_size / fragments)
  {
    fprintf(stderr,
            "Could not set requested buffer size, asked for %d got %d\n",
            buffer_size, frames * frame_size / fragments);
    buffer_size = frames * frame_size / fragments;
  }

  if ((err = snd_pcm_hw_params(device, hw_params)) < 0)
  {
    fprintf(stderr, "Error setting HW params: %s\n", snd_strerror(err));
    return 1;
  }
  return 0;
}

int main(int argc, char **argv)
{

  int err;

  if ((err = snd_pcm_open(&playback_handle, snd_device_out,
                          SND_PCM_STREAM_PLAYBACK, 0)) < 0)
  {
    fprintf(stderr, "cannot open output audio device %s: %s\n", snd_device_out,
            snd_strerror(err));
    exit(1);
  }

  if ((err = snd_pcm_open(&capture_handle, snd_device_in,
                          SND_PCM_STREAM_CAPTURE, 0)) < 0)
  {
    fprintf(stderr, "cannot open input audio device %s: %s\n", snd_device_in,
            snd_strerror(err));
    exit(1);
  }

  configure_alsa_audio(/*snd_device_out*/ playback_handle, nchannels);
  configure_alsa_audio(/*snd_device_in*/ capture_handle, nchannels);

  restarting = 1;

  int frames, inframes, outframes, frame_size;

  while (/*! exit_program*/ true)
  {
    frame_size = nchannels * (bits / 8);
    frames = buffer_size / frame_size;

    const int MIN_BUFFER_SIZE = 1024;
    const int MAX_BUFFERS = 10;

    char rdbuf[MIN_BUFFER_SIZE * MAX_BUFFERS]; /* receive buffer */
    if (restarting)
    {
      restarting = 0;
      /* drop any output we might got and stop */
      snd_pcm_drop(capture_handle);
      snd_pcm_drop(playback_handle);
      /* prepare for use */
      snd_pcm_prepare(capture_handle);
      snd_pcm_prepare(playback_handle);

      int fragments = 2;


      /* fill the whole output buffer */
      for (int i = 0; i < fragments; i += 1)
        snd_pcm_writei(playback_handle, rdbuf, frames);
    }

    while ((inframes = snd_pcm_readi(capture_handle, rdbuf, frames)) < 0)
    {
      if (inframes == -EAGAIN)
        continue;
      // by the way, writing to terminal emulator is costly if you use
      // bad emulators like gnome-terminal, so don't do this.
      fprintf(stderr, "Input buffer overrun\n");
      restarting = 1;
      snd_pcm_prepare(capture_handle);
    }
    if (inframes != frames)
      fprintf(stderr, "Short read from capture device: %d, expecting %d\n",
              inframes, frames);

    /* now processes the frames */
    // do_something(rdbuf, inframes);

    while ((outframes = snd_pcm_writei(playback_handle, rdbuf, inframes)) < 0)
    {
      if (outframes == -EAGAIN)
        continue;
      fprintf(stderr, "Output buffer underrun\n");
      restarting = 1;
      snd_pcm_prepare(playback_handle);
    }
    if (outframes != inframes)
      fprintf(stderr, "Short write to playback device: %d, expecting %d\n",
              outframes, frames);
  }

  return 0;
}
