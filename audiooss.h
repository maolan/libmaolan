#pragma once
#include <string>
#include "audioengine.h"
#include "sample.h"


class AudioOSSEngine : public AudioEngine
{
  public:
    AudioOSSEngine();
    ~AudioOSSEngine();
    void push(const int channel, const int sample);
    int pop(const int channel);
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
