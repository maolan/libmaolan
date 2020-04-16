#pragma once
#include <string>
#include <maolan/constants.h>
#include <maolan/audio/io.h>
#include <maolan/audio/oss/config.h>


namespace maolan::audio
{
class OSS : public IO
{
public:
  OSS(const std::string &device, const int &fragSize = defaultFrag, const int &channels = 2);
  ~OSS();

  static std::vector<OSSConfig *> devices;

  std::size_t channels() const;

protected:
  int *frame;
  OSSConfig *device;
};
} // namespace maolan::audio
