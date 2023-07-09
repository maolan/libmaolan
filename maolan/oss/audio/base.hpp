#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <sys/soundcard.h>

#include "maolan/audio/hw.hpp"
#include "maolan/audio/io.hpp"
#include "maolan/constants.hpp"


namespace maolan::audio
{
class OSS : public maolan::audio::HW
{
public:
  OSS(const std::string &device, const int &fragSize = defaultFrag,
      const int &sampleSize = 4);
  ~OSS();

  virtual std::size_t channels() const;
  virtual nlohmann::json json();

protected:
  int8_t *_bytes;
  int _format;
  int _frag;
  int _sampleCount;
  oss_audioinfo _audioInfo;
  audio_buf_info _bufferInfo;
  std::size_t _sampleSize;
};
} // namespace maolan::audio
