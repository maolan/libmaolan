#include <sys/soundcard.h>
#include "maolan/audio/oss/config.hpp"


using namespace maolan::audio;


OSSConfig::OSSConfig()
  : format{AFMT_S32_NE}
  , frag{16}
{
  name = "OSS";
  sampleSize = 4;
}
