#include <iostream>
#include <maolan/audio/clip.hpp>
#include <maolan/audio/oss/out.hpp>
#include <maolan/audio/track.hpp>
#include <maolan/engine.hpp>


namespace lv2 = maolan::plugin::lv2;
using namespace maolan::audio;


int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <plugin uri>" << std::endl;
    return 1;
  }
  OSSOut out("/dev/dsp", 2);
  Track trackp("play", 2);
  Clip clip("../data/stereo.wav", 0, 10000000, 0, &trackp);
  out.connect(&trackp);

  /* Background threads + main thread
   */
  maolan::Engine::init();
  std::cerr << "Playing ...";
  maolan::Engine::play();
  std::this_thread::sleep_for(std::chrono::seconds(14));
  std::cerr << " done\n";
  std::cerr << "Stopping ...";
  maolan::Engine::stop();
  std::cerr << " done\n";
  std::cerr << "Exiting ...";
  maolan::Engine::quit();
  std::cerr << " done\n";

  /* Only main thread (foreground)
  while (true)
  {
    for (auto io = IO::begin(); io != nullptr; io = io->next())
    {
      io->setup();
    }
    for (auto io = IO::begin(); io != nullptr; io = io->next())
    {
      io->fetch();
    }
    for (auto io = IO::begin(); io != nullptr; io = io->next())
    {
      io->process();
    }
    auto playhead = IO::playHead();
    IO::playHead(playhead + Config::audioBufferSize);
  }
  */
  return 0;
}
