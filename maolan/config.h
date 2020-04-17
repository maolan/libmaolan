#pragma once
#include <memory>
#include <vector>
#include <maolan/bpm.h>


namespace maolan
{
class Config
{
public:
  static std::size_t audioBufferSize;
  static unsigned samplerate;
  static std::size_t bpm;
  std::size_t count;
  static std::vector<BPM> bpms;
};
} // namespace maolan
