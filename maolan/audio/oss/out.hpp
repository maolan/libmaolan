#pragma once
#include <nlohmann/json.hpp>
#include <string>

#include "maolan/audio/input.hpp"
#include "maolan/audio/oss/base.hpp"


namespace maolan::audio
{
template <typename T> class OSSOut : public OSS
{
public:
  OSSOut(const std::string &device, const int &frag = defaultFrag);

  virtual void fetch();
  virtual void process();
  void convertToRaw();
};
} // namespace maolan::audio
