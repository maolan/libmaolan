#include "maolan/alsa/audio/out.hpp"


int main()
{
  maolan::audio::ALSAOut<int32_t> out{"AlsaOut", "default"};
  return 0;
}
