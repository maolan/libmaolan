/* vim: set syntax=cpp: */
#pragma once
#include <maolan/audioio.h>


class AudioConnection
{
  public:
    AudioConnection();
    AudioConnection(AudioIO *connectTo, std::size_t &ch);

    AudioIO * get();
    void target(AudioIO *connectTo, std::size_t &ch);
    AudioChunk pull();

  protected:
    AudioIO *to;
    std::size_t channel;
};
