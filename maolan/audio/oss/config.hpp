#pragma once
#include <string>
#include <sys/soundcard.h>
#include "maolan/config.hpp"


namespace maolan::audio
{
class OSSConfig : public Config
{
public:
  OSSConfig();

  int fd;
  int format;
  int frag;
  int fragSize;
  int channels;
  std::size_t audioChannels;
  oss_audioinfo audioInfo;
  audio_buf_info bufferInfo;
};
} // namespace maolan::audio
