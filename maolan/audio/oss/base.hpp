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
  OSS(const std::string &device, const int &fragSize = defaultFrag, const int &sampleSize = 4);
  ~OSS();

  virtual std::size_t channels() const;

protected:
  int8_t *bytes;
  OSSConfig *device;
};
} // namespace maolan::audio
