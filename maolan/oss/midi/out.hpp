#pragma once
#include <string>

#include "maolan/oss/midi/base.hpp"


namespace maolan::midi
{
class OSSOut : public OSS
{
public:
  OSSOut(const std::string &name, const std::string &device);

  virtual void fetch();
  virtual void process();
};
} // namespace maolan::midi
