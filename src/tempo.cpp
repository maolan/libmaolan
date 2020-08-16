#include <maolan/config.h>
#include <maolan/tempo.h>


using namespace maolan;


Tempo::Tempo(const unsigned &b, const std::size_t &t) : bpm{b}, time{t}
{
  float audio = (float)Config::samplerate * 60.0;
  float midi = (float)bpm * (float)Config::division;
  ratio = audio / midi;
}
