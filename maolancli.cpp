#include <iostream>
#include <maolan/worker.h>
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

  auto worker = new maolan::Worker();
  auto worker2 = new maolan::Worker();
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cerr << "Playing ...";
  IO::play();
  std::this_thread::sleep_for(std::chrono::seconds(20));
  std::cerr << " done\n";
  IO::stop();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cerr << "Exiting ...";
  IO::quit();
  std::cerr << " done\n";
  delete worker;
  delete worker2;


  maolan::audio::Plugin::destroyWorld();
  return 0;
}
