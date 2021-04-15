#include <iostream>

#include <maolan/config.hpp>
#include <maolan/engine.hpp>


int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <session dir>" << std::endl;
    return 1;
  }
  maolan::Engine::load(argv[1]);
  maolan::Engine::init();
  maolan::Engine::save();
  return 0;
}
