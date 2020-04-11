#pragma once
#include <string>
#include <maolan/audio/oss.h>


namespace maolan::audio
{
class OSSIn : public OSS
{
public:
  OSSIn(const std::string &device, const std::size_t &channels, const std::size_t &frag = defaultFrag);

  void fetch();
  void process();
};
} // namespace maolan::audio
