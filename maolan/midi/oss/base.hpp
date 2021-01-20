#pragma once
#include "maolan/midi/buffer.hpp"
#include "maolan/midi/io.hpp"
#include "maolan/midi/oss/config.hpp"


namespace maolan::midi
{
class OSSMIDI : public IO
{
public:
  OSSMIDI(const std::string &device);
  ~OSSMIDI();

protected:
  OSSMIDIConfig *device;
  Buffer data;
};
} // namespace maolan::midi
