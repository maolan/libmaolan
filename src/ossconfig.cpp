#include <sys/soundcard.h>
#include <maolan/ossconfig.h>


OSSConfig::OSSConfig()
{
  count = 1;
  format = AFMT_S32_NE;
  samplerate = 48000;
  frag = 16;
  fragSize = 0;
  bpm = 120;
}
