#pragma once
#include "audioengine.h"


class AudioOSSEngine : public AudioEngine
{
  public:
    void push() {}
    void pop() {}
    void consume() {}
    void produce() {}
};
