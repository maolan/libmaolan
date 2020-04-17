#include <maolan/config.h>

using namespace maolan;

std::size_t Config::audioBufferSize = 0;
unsigned Config::samplerate = 48000;
std::size_t Config::bpm = 120;
std::vector<BPM> Config::bpms;
