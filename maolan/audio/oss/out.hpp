#pragma once
#include <string>
#include "maolan/audio/connectable.hpp"
#include "maolan/audio/input.hpp"
#include "maolan/audio/oss/base.hpp"


namespace maolan::audio
{
class OSSOut : public OSS, public Connectable
{
public:
  OSSOut(const std::string &device, const int &frag = defaultFrag);

  virtual void fetch();
  virtual void process();
  void convertToRaw();
  void play(int *frame, std::size_t dataSize);
};
} // namespace maolan::audio
