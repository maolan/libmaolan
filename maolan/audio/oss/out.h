#pragma once
#include <string>
#include <maolan/audio/connectable.h>
#include <maolan/audio/input.h>
#include <maolan/audio/oss/base.h>


namespace maolan::audio
{
class OSSOut : public OSS, public Connectable
{
public:
  OSSOut(const std::string &device, const int &channels, const int &frag = defaultFrag);

  virtual void fetch();
  virtual void process();
  void convertToRaw();
  void play(int *frame, std::size_t dataSize);
};
} // namespace maolan::audio
