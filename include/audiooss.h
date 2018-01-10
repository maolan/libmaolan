#pragma once
#include <string>
#include "audioengine.h"


class AudioOSSEngine : public AudioEngine
{
  public:
    AudioOSSEngine();
    ~AudioOSSEngine();
    void push(const int channel);
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
