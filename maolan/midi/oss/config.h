#pragma once
#include <string>
#include <maolan/config.h>


namespace maolan::midi
{
class OSSMIDIConfig : public Config
{
public:
  OSSMIDIConfig();

  int fd;
  std::string device;
};
} // namespace maolan::midi
