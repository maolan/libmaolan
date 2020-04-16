#include <iostream>
#include <maolan/engine.h>
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

  maolan::Engine::init();
  std::cerr << "Playing ...";
  maolan::Engine::play();
  std::this_thread::sleep_for(std::chrono::seconds(20));
  std::cerr << " done\n";
  std::cerr << "Stopping ...";
  maolan::Engine::stop();
  std::cerr << " done\n";
  std::this_thread::sleep_for(std::chrono::seconds(10));
  std::cerr << "Exiting ...";
  maolan::Engine::quit();
  std::cerr << " done\n";


  maolan::audio::Plugin::destroyWorld();
  return 0;
}
