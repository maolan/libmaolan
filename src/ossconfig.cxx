#include <sys/soundcard.h>
#include <maolan/ossconfig>


OSSConfig::OSSConfig()
{
  count = 1;
  format = AFMT_S32_NE;
  samplerate = 44100;
  frag = 16;
  fragSize = 0;
}
