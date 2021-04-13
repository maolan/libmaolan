#pragma once
#include "maolan/midi/input.hpp"
#include "maolan/midi/oss/base.hpp"
#include <string>


namespace maolan::midi
{
class OSSMIDIOut : public OSSMIDI
{
public:
  OSSMIDIOut(const std::string &device);

  virtual void fetch();
  virtual void process();
};
} // namespace maolan::midi
