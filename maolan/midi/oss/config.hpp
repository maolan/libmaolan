#pragma once
#include "maolan/config.hpp"
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
