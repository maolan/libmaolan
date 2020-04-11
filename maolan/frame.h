#pragma once
#include <vector>
#include <maolan/audio/buffer.h>


namespace maolan
{
class Frame
{
  public:
    Frame(std::size_t audioSize, std::size_t controlSize);
    ~Frame();

    std::vector<audio::Buffer> audioBuffer;
    float *controls;

};
} // namespace maolan
