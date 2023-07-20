#include <cstdint>
#include <filesystem>
#include <iostream>
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
  maolan::Engine::init();
  if (maolan::Engine::load(argv[1]) == nullptr)
  {
    return 1;
  }

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
  return 0;
}
