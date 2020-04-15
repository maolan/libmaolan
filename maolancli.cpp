#include <iostream>
#include <maolan/audio/clip.h>
#include <maolan/audio/ossout.h>
#include <maolan/audio/track.h>


using namespace maolan::audio;


int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <plugin uri>" << std::endl;
    return 1;
  }
  OSSOut out("/dev/dsp", 1, 8);
  Track trackp("play", 1);
  Clip clip("../data/mono.wav", 0, 10000000, 0, &trackp);
  out.connect(&trackp);
  std::cout << "Playing ..." << std::endl;
  while (true)
  {
    auto task = IO::task();
    if (task == nullptr) { break; }
    task->work();
  }

  maolan::audio::Plugin::destroyWorld();
  return 0;
}
