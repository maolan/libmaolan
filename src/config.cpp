#include <maolan/config.h>


using namespace maolan;


std::size_t Config::audioBufferSize = 0;
unsigned Config::samplerate = 48000;
std::size_t Config::tempoIndex = 0;
std::vector<Tempo> Config::tempos = {120};
std::uint16_t Config::division = 192;
