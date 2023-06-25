#pragma once
#include "maolan/midi/buffer.hpp"
#include "maolan/midi/io.hpp"
#include "maolan/oss/midi/config.hpp"


namespace maolan::midi
{
class OSSMIDI : public IO
{
public:
  OSSMIDI(const std::string &device);
  ~OSSMIDI();

protected:
  static std::vector<OSSMIDIConfig *> _devices;

  OSSMIDIConfig *device;
  Buffer data;
};
} // namespace maolan::midi
