#pragma once
#include <maolan/tempo.h>
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
  static std::size_t tempoIndex;
  static std::vector<Tempo> tempos;
  static unsigned division;
  std::size_t count;
  std::string name;
  std::string device;
};
} // namespace maolan
