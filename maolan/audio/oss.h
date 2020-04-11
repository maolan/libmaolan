#pragma once
#include <maolan/constants.h>
#include <maolan/audio/io.h>
#include <maolan/audio/ossconfig.h>
#include <string>


namespace maolan::audio
{
class OSS : public IO
{
public:
  OSS(const std::string &device, const std::size_t &fragSize = defaultFrag);
  ~OSS();

  static std::vector<OSSConfig *> devices;

  std::size_t channels() const;

protected:
  int *frame;
  OSSConfig *device;
};
} // namespace maolan::audio
