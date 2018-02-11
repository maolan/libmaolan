#include <sys/soundcard.h>
#include <maolan/config>


int Config::format = AFMT_S32_NE;
int Config::samplerate = 44100;
size_t Config::frag = 4;
size_t Config::fragSize = 16;
size_t Config::audioChunkSize = 8;
