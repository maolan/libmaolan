/* vim: set syntax=cpp: */
#pragma once
#include <maolan/audio/io.h>
#include <maolan/audio/ossconfig.h>
#include <string>


namespace maolan
{
namespace audio
{
class OSS : public IO
{
public:
  OSS(const std::string &device);
  ~OSS();

  static std::vector<OSSConfig *> devices;

  std::size_t channels() const;

protected:
  int *frame;
  OSSConfig *device;
};
} // namespace audio
} // namespace maolan
