/* vim: set syntax=cpp: */
#pragma once
#include <memory>

namespace maolan
{
namespace audio
{
class BufferData
{
public:
  BufferData(const size_t &size);
  ~BufferData();

  float *data;
};


typedef std::shared_ptr<BufferData> Buffer;
} // namespace audio
} // namespace maolan
