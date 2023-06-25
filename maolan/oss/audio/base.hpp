#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <sys/soundcard.h>

#include "maolan/constants.hpp"
#include "maolan/oss/io.hpp"


namespace maolan::audio
{
class OSS : public maolan::audio::oss::IO
{
public:
  OSS(const std::string &device, const int &fragSize = defaultFrag,
      const int &sampleSize = 4);
  ~OSS();

  virtual std::size_t channels() const;
  virtual nlohmann::json json();

protected:
  int8_t *_bytes;
  int _fd;
  int _format;
  int _frag;
  int _sampleCount;
  oss_audioinfo _audioInfo;
  audio_buf_info _bufferInfo;
  std::string _device;
  std::size_t _sampleSize;
};
} // namespace maolan::audio
