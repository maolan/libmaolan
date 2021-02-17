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
  int frag;
  int fragSize;
  int channels;
  std::size_t audioChannels;
};
} // namespace maolan::audio
