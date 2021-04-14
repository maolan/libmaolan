#include <cstdint>
#include <iostream>

#include <maolan/audio/clip.hpp>
#include <maolan/audio/oss/out.hpp>
#include <maolan/audio/track.hpp>
#include <maolan/config.hpp>
#include <maolan/engine.hpp>
#include <maolan/io.hpp>
#include <unistd.h>


using namespace maolan;


int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <session dir>" << std::endl;
    return 1;
  }
  Config::root = argv[1];
  audio::OSSOut<int32_t> out("/dev/dsp");
  // audio::Track trackp("play", 2);
  // audio::Clip clip("../data/audio/stereo.wav", &trackp, 0, 10000000, 0);
  // out.connect(&trackp);
  Engine::init(0);

  std::cout << "Playing ...\n";
  while (true)
  {
    Engine::setup();
    Engine::fetch();
    Engine::process();
    auto playhead = IO::playHead();
    IO::playHead(playhead + Config::audioBufferSize);
  }
  return 0;
}
