#pragma once
#include <memory>

namespace maolan::audio
{
class BufferData
{
public:
  BufferData(const size_t &size);
  ~BufferData();

  float *data;
};


typedef std::shared_ptr<BufferData> Buffer;
} // namespace maolan::audio
