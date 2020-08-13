#pragma once
#include <maolan/midi/oss/base.h>
#include <string>


namespace maolan::midi
{
class OSSMIDIIn : public OSSMIDI
{
public:
  OSSMIDIIn(const std::string &device);

  virtual void setup();
  virtual void fetch();
  virtual void process();
  virtual Buffer pull();
};
} // namespace maolan::midi
