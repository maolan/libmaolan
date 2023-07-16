#pragma once
#include <nlohmann/json.hpp>
#include <string>

#include "maolan/audio/input.hpp"
#include "maolan/oss/audio/base.hpp"


namespace maolan::audio
{
template <typename T> class OSSOut : public OSS
{
public:
  OSSOut(const std::string &name, const std::string &device,
         const int &frag = defaultFrag);

  virtual void fetch();
  virtual void process();
  virtual void writehw();
};
} // namespace maolan::audio
