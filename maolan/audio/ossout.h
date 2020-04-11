#pragma once
#include <string>
#include <maolan/audio/connectable.h>
#include <maolan/audio/input.h>
#include <maolan/audio/oss.h>


namespace maolan::audio
{
class OSSOut : public OSS, public Connectable
{
public:
  OSSOut(const std::string &device, const std::size_t &channels, const std::size_t &frag = defaultFrag);

  void fetch();
  void process();
  void convertToRaw();
  void play(int *frame, std::size_t dataSize);
};
} // namespace maolan::audio
