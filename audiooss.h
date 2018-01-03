#pragma once
#include <string>
#include "audioengine.h"
#include "sample.h"


class AudioOSSEngine : public AudioEngine
{
  public:
    AudioOSSEngine();
    ~AudioOSSEngine();
    void push(Sample sample);
    Sample & pop();
    void consume();
    void produce();

  protected:
    int fd;
    string device;
    int channels;
    int format;
    int samplerate;
};
