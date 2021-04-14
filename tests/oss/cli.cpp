#include <cstdint>
#include <iostream>
#include <libgen.h>
#include <unistd.h>

#include <maolan/audio/clip.hpp>
#include <maolan/audio/oss/out.hpp>
#include <maolan/audio/track.hpp>
#include <maolan/config.hpp>
#include <maolan/engine.hpp>
#include <maolan/midi/clip.hpp>
#include <maolan/midi/track.hpp>


using namespace maolan::audio;


int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <session dir>" << std::endl;
    return 1;
  }
  maolan::Config::root = argv[1];

  /* Background threads + main thread
   */
  maolan::Engine::load();
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

  // while (true)
  // {
  // for (auto io = IO::begin(); io != nullptr; io = io->next())
  // {
  // io->setup();
  // }
  // for (auto io = IO::begin(); io != nullptr; io = io->next())
  // {
  // io->fetch();
  // }
  // for (auto io = IO::begin(); io != nullptr; io = io->next())
  // {
  // io->process();
  // }
  // auto playhead = IO::playHead();
  // IO::playHead(playhead + maolan::Config::audioBufferSize);
  // }
  return 0;
}
