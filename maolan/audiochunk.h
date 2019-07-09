/* vim: set syntax=cpp: */
#pragma once
#include <memory>

namespace maolan
{
  namespace audio
  {
    class AudioChunkData
    {
    public:
    AudioChunkData(const size_t &size);
    ~AudioChunkData();

    float *data;
    };


    typedef std::shared_ptr<AudioChunkData> AudioChunk;
  }
}
