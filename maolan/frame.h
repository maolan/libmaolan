#pragma once
#include <maolan/audio/buffer.h>
#include <vector>


namespace maolan
{
class Frame
{
public:
  Frame(std::size_t audioSize, std::size_t controlSize);

  std::vector<audio::Buffer> audioBuffer;
  std::vector<float> controls;
};
} // namespace maolan
