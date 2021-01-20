#pragma once
#include <string>
#include "maolan/audio/oss/base.hpp"


namespace maolan::audio
{
class OSSIn : public OSS
{
public:
  OSSIn(const std::string &device, const int &channels,
        const int &frag = defaultFrag);

  void fetch();
  void process();
};
} // namespace maolan::audio
