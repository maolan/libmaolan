#pragma once
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "maolan/tempo.hpp"


namespace maolan
{
class Config
{
public:
  static std::size_t audioBufferSize;
  static unsigned samplerate;
  static std::size_t tempoIndex;
  static std::vector<Tempo> tempos;
  static std::uint16_t division;
  static std::filesystem::path root;

  std::size_t count;
  std::string name;
  std::string device;
  std::size_t sampleSize;
};
} // namespace maolan
