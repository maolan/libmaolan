#include "maolan/config.hpp"
#include "maolan/tempo.hpp"


using namespace maolan;


Tempo::Tempo(const unsigned &b, const std::size_t &t)
  : bpm{b}
  , time{t}
{
  if (Config::samplerate != 0 && bpm != 0 && Config::division != 0)
  {
    float audio = (float)Config::samplerate * 60.0;
    float midi = (float)bpm * (float)Config::division;
    a2m = audio / midi;
    tick = time / a2m;
    spt = audio / (float)bpm;
  }
}
