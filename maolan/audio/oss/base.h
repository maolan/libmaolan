#pragma once
#include <maolan/audio/io.h>
#include <maolan/audio/oss/config.h>
#include <maolan/constants.h>
#include <string>


namespace maolan::audio
{
class OSS : public IO
{
public:
  OSS(const std::string &device, const int &fragSize = defaultFrag,
      const int &channels = 2);
  ~OSS();

  virtual std::size_t channels() const;

protected:
  int *frame;
  OSSConfig *device;
};
} // namespace maolan::audio
