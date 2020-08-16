#pragma once
#include <maolan/config.h>
#include <string>


namespace maolan::audio
{
class OSSConfig : public Config
{
public:
  OSSConfig();

  int fd;
  int format;
  std::size_t audioChannels;
  int frag;
  int fragSize;
  int channels;
};
} // namespace maolan::audio
