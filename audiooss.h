#pragma once
#include "audioengine.h"
#include "sample.h"


class AudioOSSEngine : public AudioEngine
{
  public:
    AudioOSSEngine();
    ~AudioOSSEngine();
    void push(Sample &sample);
    Sample pop();
    void consume();
    void produce();

  protected:
    int fd;
};
