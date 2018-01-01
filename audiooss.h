#pragma once
#include "audioengine.h"


class AudioOSSEngine : public AudioEngine
{
  public:
    int read() {return 0;}
    void write() {}
};
