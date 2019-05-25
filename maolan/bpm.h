#pragma once
#include <stddef.h>


class BPM
{
  public:
    BPM(const int &bpm = 120, const size_t tick = 0);

    int bpm;
    float bps;
    size_t tick;
};
