#pragma once
#include <string>
#include <nlohmann/json.hpp>

#include "maolan/audio/connectable.hpp"
#include "maolan/audio/input.hpp"
#include "maolan/audio/oss/base.hpp"


namespace maolan::audio
{
template <class T>
class OSSOut : public OSS, public Connectable
{
public:
  OSSOut(const std::string &device, const int &frag = defaultFrag);

  virtual void fetch();
  virtual void process();
  virtual nlohmann::json connections();
  void convertToRaw();
};
} // namespace maolan::audio
