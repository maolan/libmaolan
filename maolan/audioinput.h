/* vim: set syntax=cpp: */
#pragma once
#include <vector>
#include <maolan/audioconnection.h>
#include <maolan/audiochunk.h>


class AudioInput : public AudioIO
{
  public:
    AudioInput();

    void add(AudioIO *to, std::size_t ch);
    void fetch();
    void process();
    std::size_t channels() const;
    AudioChunk pull();

  protected:
    std::vector<AudioConnection> connections;
};
