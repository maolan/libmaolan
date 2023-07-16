#include "maolan/alsa/audio/out.hpp"
// #include "maolan/oss/audio/out.hpp"
#include "maolan/audio/clip.hpp"
#include "maolan/io.hpp"


int main()
{
  maolan::audio::ALSAOut<int32_t> out{"AlsaOut", "default"};
  // maolan::audio::OSSOut<int32_t> out{"OssOut", "/dev/dsp"};
  maolan::audio::Clip c{"/usr/home/meka/repos/maolan/libmaolan/data/audio/stereo.wav"};
  out.connect(&c);
  while(true)
  {
    c.fetch();
    c.process();
    out.fetch();
    out.process();
    out.writehw();
  }
  return 0;
}
