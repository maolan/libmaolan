#include "maolan/alsa/audio/in.hpp"
#include "maolan/alsa/audio/out.hpp"


int main()
{
  maolan::audio::ALSAIn<int32_t> in{"AlsaIn", "default"};
  maolan::audio::ALSAOut<int32_t> out{"AlsaOut", "default"};
  return 0;
}
