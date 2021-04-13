#pragma once
#include "maolan/audio/buffer.hpp"
#include "maolan/midi/buffer.hpp"
#include <vector>


namespace maolan
{
class Frame
{
public:
  Frame(const std::size_t &audioSize, const std::size_t &midiSize,
        const std::size_t &controlSize);

  std::vector<audio::Buffer> audio;
  std::vector<midi::Buffer> midi;
  std::vector<float> control;
};
} // namespace maolan
