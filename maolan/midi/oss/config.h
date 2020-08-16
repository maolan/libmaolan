#pragma once
#include <maolan/config.h>
#include <string>


namespace maolan::midi
{
class OSSMIDIConfig : public Config
{
public:
  OSSMIDIConfig();

  int fd;
};
} // namespace maolan::midi
