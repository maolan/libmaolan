#include <iostream>
#include <maolan/engine.h>
#include <maolan/audio/clip.h>
#include <maolan/audio/ossout.h>
#include <maolan/audio/track.h>


using namespace maolan;


int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <plugin uri>" << std::endl;
    return 1;
  }
  audio::OSSOut out("/dev/dsp", 1, 8);
  audio::Track trackp("play", 1);
  audio::Clip clip("../data/mono.wav", 0, 10000000, 0, &trackp);
  out.connect(&trackp);

  Engine::init();
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cerr << "Playing ...";
  Engine::play();
  std::this_thread::sleep_for(std::chrono::seconds(20));
  std::cerr << " done\n";
  Engine::stop();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cerr << "Exiting ...";
  Engine::quit();
  std::cerr << " done\n";


  maolan::audio::Plugin::destroyWorld();
  return 0;
}
