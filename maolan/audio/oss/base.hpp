#pragma once
#include <nlohmann/json.hpp>
#include <string>

#include "maolan/audio/io.hpp"
#include "maolan/audio/oss/config.hpp"
#include "maolan/constants.hpp"


namespace maolan::audio
{
class OSS : public IO
{
public:
  OSS(const std::string &device, const int &fragSize = defaultFrag,
      const int &sampleSize = 4);
  ~OSS();

  virtual std::size_t channels() const;
  virtual nlohmann::json json();

protected:
  static std::vector<OSSConfig *> _devices;

  int8_t *_bytes;
  OSSConfig *_device;
};
} // namespace maolan::audio
