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
  static size_t audioBufferSize;
  static unsigned samplerate;
  static size_t tempoIndex;
  static std::vector<Tempo> tempos;
  static std::uint16_t division;
  static std::filesystem::path root;
};
} // namespace maolan
