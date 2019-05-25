#include <maolan/bpm.h>


BPM::BPM(const int &b, const size_t t)
  : bpm{b}
  , tick{t}
{
  bps = bpm / 60.0;
}
