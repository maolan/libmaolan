#include <maolan/audio/oss/config.h>
#include <sys/soundcard.h>


using namespace maolan::audio;


OSSConfig::OSSConfig() : format{AFMT_S32_NE}, frag{16}, fragSize{0}
{
  name = "OSS";
}
