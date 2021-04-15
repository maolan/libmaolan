#include <cstdint>
#include <iostream>
#include <libgen.h>
#include <unistd.h>

#include <maolan/config.hpp>
#include <maolan/engine.hpp>
#include <maolan/io.hpp>


int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <session dir>" << std::endl;
    return 1;
  }
  maolan::Engine::load(argv[1]);
  // If single threaded mode, use init(0)
  maolan::Engine::init();

  // Multi-threaded mode
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

  // Single-threaded mode
  // while (true)
  // {
  //   maolan::Engine::setup();
  //   maolan::Engine::fetch();
  //   maolan::Engine::process();
  //   auto playhead = maolan::IO::playHead();
  //   maolan::IO::playHead(playhead + maolan::Config::audioBufferSize);
  // }
  return 0;
}
