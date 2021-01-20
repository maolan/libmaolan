#pragma once
#include <string>
#include "maolan/config.hpp"


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
