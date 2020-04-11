#include <maolan/audio/ossconfig.h>
#include <sys/soundcard.h>


using namespace maolan::audio;


OSSConfig::OSSConfig()
  : count{1}
  , format{AFMT_S32_NE}
  , frag{16}
  , fragSize{0}
{}
