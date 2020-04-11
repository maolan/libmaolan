#pragma once
#include <string>
#include <maolan/config.h>


namespace maolan::audio
{
class OSSConfig : public Config
{
public:
  OSSConfig();

  int fd;
  int format;
  std::size_t audioChannels;
  std::size_t frag;
  std::size_t fragSize;
  std::size_t count;
  std::string device;
};
} // namespace maolan::audio
