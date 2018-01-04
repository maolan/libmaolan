#pragma once
#include <string>
#include "audioengine.h"
#include "sample.h"


class AudioOSSEngine : public AudioEngine
{
  public:
    AudioOSSEngine();
    ~AudioOSSEngine();
    void push(const Sample sample);
    Sample & pop(const int channel);
    void consume();
    void produce();
    void sync();

  protected:
    int fd;
    string device;
    int channels;
    int format;
    int samplerate;
};
