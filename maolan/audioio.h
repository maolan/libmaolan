/* vim: set syntax=cpp: */
#pragma once
#include <string>
#include <vector>
#include <maolan/io.h>
#include <maolan/audiochunk.h>


class AudioIO : public IO
{
  public:
    AudioIO(const size_t &outs = 0, bool front = false);
    ~AudioIO();

    virtual std::size_t channels() const = 0;
    AudioChunk pull(const unsigned &channel);

  protected:
    std::vector<AudioChunk> outputs;
};
