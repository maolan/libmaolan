#pragma once
#include <vector>
#include <maolan/audio/buffer.h>


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
