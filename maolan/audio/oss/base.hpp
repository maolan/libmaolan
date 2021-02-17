#pragma once
#include <string>
#include "maolan/audio/io.hpp"
#include "maolan/audio/oss/config.hpp"
#include "maolan/constants.hpp"


namespace maolan::audio
{
class OSS : public IO
{
public:
  OSS(const std::string &device, const int &fragSize = defaultFrag);
  ~OSS();

  virtual std::size_t channels() const;

protected:
  int *frame;
  OSSConfig *device;
};
} // namespace maolan::audio
