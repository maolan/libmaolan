#pragma once
#include <vector>
#include "audiochunk.h"


class AudioIO
{
  public:
    AudioIO(const size_t &size);

    virtual void fetch() = 0;
    virtual std::size_t channels() const = 0;
    AudioChunk pull(const unsigned &channel);

  protected:
    std::vector<AudioChunk> outputs;
};

