#pragma once
#include <maolan/midi/buffer.h>
#include <maolan/midi/io.h>
#include <maolan/midi/oss/config.h>


namespace maolan::midi
{
class OSSMIDI : public MIDIIO
{
public:
  OSSMIDI(const std::string &device);
  ~OSSMIDI();

protected:
  OSSMIDIConfig *device;
  Buffer data;
};
} // namespace maolan::midi::oss
