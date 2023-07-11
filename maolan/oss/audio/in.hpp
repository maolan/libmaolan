#pragma once
#include "maolan/oss/audio/base.hpp"
#include <string>


struct pollfd;

namespace maolan::audio
{
template <typename T> class OSSIn : public OSS
{
public:
  OSSIn(const std::string &name, const std::string &device,
        const int &frag = defaultFrag);

  virtual void readhw();
  virtual void process();
};
} // namespace maolan::audio
