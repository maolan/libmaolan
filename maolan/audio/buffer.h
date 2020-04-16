#pragma once
#include <memory>
#include <vector>

namespace maolan::audio
{
class BufferData
{
  public:
    BufferData(const size_t &size);

    std::vector<float> data;

  private:
    BufferData();
};


typedef std::shared_ptr<BufferData> Buffer;
} // namespace maolan::audio
