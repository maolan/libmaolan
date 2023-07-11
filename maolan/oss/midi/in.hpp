#pragma once
#include <string>

#include "maolan/oss/midi/base.hpp"

namespace maolan::midi
{
class OSSIn : public OSS
{
public:
  OSSIn(const std::string &name, const std::string &device);

  virtual void setup();
  virtual void readhw();
  virtual void process();
  virtual Buffer pull(const std::size_t &channel);
};
} // namespace maolan::midi
