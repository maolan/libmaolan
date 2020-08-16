#include <maolan/config.h>
#include <maolan/tempo.h>


using namespace maolan;


Tempo::Tempo(const unsigned &b, const std::size_t &t) : bpm{b}, time{t}
{
  if (Config::samplerate != 0 && bpm != 0 && Config::division != 0)
  {
    float audio = (float)Config::samplerate * 60.0;
    float midi = (float)bpm * (float)Config::division;
    ratio = audio / midi;
    tick = time / ratio;
  }
}
