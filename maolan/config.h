#pragma once
#include <maolan/bpm.h>
#include <memory>
#include <string>
#include <vector>


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
  std::string name;
  std::string device;
};
} // namespace maolan
