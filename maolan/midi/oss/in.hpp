#pragma once
#include <string>
#include "maolan/midi/oss/base.hpp"


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
