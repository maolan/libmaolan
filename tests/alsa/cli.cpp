#include <iostream>
#include <maolan/audio/alsa/out.h>
#include <maolan/audio/clip.h>
#include <maolan/audio/track.h>
#include <maolan/engine.h>


using namespace maolan::audio;


int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <plugin uri>" << std::endl;
    return 1;
  }
  ALSAOut out("default", 2, 512);
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
  }
  */

  maolan::plugin::lv2::Plugin::destroyWorld();
  return 0;
}
