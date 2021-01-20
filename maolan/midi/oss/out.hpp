#pragma once
#include <string>
#include "maolan/midi/connectable.hpp"
#include "maolan/midi/input.hpp"
#include "maolan/midi/oss/base.hpp"


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
