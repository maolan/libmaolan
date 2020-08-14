#pragma once
#include <maolan/midi/connectable.h>
#include <maolan/midi/input.h>
#include <maolan/midi/oss/base.h>
#include <string>


namespace maolan::midi
{
class OSSMIDIOut : public OSSMIDI, public Connectable
{
public:
  OSSMIDIOut(const std::string &device);

  virtual void fetch();
  virtual void process();
};
} // namespace maolan::midi
