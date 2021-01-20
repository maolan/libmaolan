#pragma once
#include <memory>


namespace maolan
{
class Tempo
{
public:
  Tempo(const unsigned &bpm = 120, const std::size_t &time = 0);
  unsigned bpm;
  float ratio;
  std::size_t time;
  std::size_t tick;
};
} // namespace maolan
