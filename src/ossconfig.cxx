#include <sys/soundcard.h>
#include <maolan/ossconfig>


OSSConfig::OSSConfig()
{
  format = AFMT_S32_NE;
  samplerate = 44100;
  frag = 16;
  fragSize = 0;
}


OSSConfig::OSSConfig(const OSSConfig &config)
{
  fd = config.fd;
  format = config.format;
  samplerate = config.samplerate;
  frag = config.frag;
  fragSize = config.fragSize;
  device = config.device;
}

