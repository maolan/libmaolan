/* vim: set syntax=cpp: */
#pragma once
#include <memory>

namespace maolan
{
  namespace audio
  {
    class ChunkData
    {
    public:
    ChunkData(const size_t &size);
    ~ChunkData();

    float *data;
    };


    typedef std::shared_ptr<ChunkData> Chunk;
  }
}
