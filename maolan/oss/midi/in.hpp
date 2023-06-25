#pragma once
#include <string>

#include "maolan/oss/midi/base.hpp"

namespace maolan::midi
{
class OSSMIDIIn : public OSSMIDI
{
public:
  OSSMIDIIn(const std::string &device);

  virtual void setup();
  virtual void fetch();
  virtual void process();
  virtual Buffer pull(const std::size_t &channel);
};
} // namespace maolan::midi
