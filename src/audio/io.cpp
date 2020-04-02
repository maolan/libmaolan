#include <maolan/audio/io.h>


using namespace maolan::audio;


IO::IO(const size_t &chs, const bool &front, const bool &reg)
    : maolan::IO("", front, reg)
{
  if (chs > 0)
  {
    outputs.resize(chs);
  }
}


Buffer IO::pull(const unsigned &channel) { return outputs[channel]; }
