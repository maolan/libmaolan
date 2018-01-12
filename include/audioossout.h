#pragma once
#include <string>
#include "audioio.h"


class AudioOSSOut : public AudioIO
{
  public:
    AudioOSSOut(const std::size_t &size);
    ~AudioOSSOut();

    void sync() const;

  protected:
    int fd;
    int samplerate;
    int format;
};
