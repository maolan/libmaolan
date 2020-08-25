#include <chrono>
#include <iostream>
#include <maolan/audio/oss/out.h>
#include <maolan/engine.h>
#include <maolan/midi/clip.h>
#include <maolan/midi/oss/in.h>
#include <maolan/midi/oss/out.h>
#include <maolan/midi/track.h>
#include <thread>

namespace audio = maolan::audio;
namespace midi = maolan::midi;


int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <midi file>" << std::endl;
    return 1;
  }

  audio::OSSOut out("/dev/dsp", 2, 4);
  // midi::Track track("Something", 2);
  // midi::Clip clip(argv[1], nullptr);
  // midi::OSSMIDIOut midiOut("/dev/umidi0.0");
  // midiOut.connect(&track);
  // clip.save();
  midi::Clip clip2("/tmp/clip.mid", nullptr);

  // maolan::Engine::init();
  // std::cerr << "Playing ...";
  // maolan::Engine::play();
  // std::this_thread::sleep_for(std::chrono::seconds(14));
  // std::cerr << " done\n";
  // std::cerr << "Stopping ...";
  // maolan::Engine::stop();
  // std::cerr << " done\n";
  // std::cerr << "Saving ...";
  // maolan::Engine::save();
  // std::cerr << " done\n";
  // std::cerr << "Exiting ...";
  // maolan::Engine::quit();
  // std::cerr << " done\n";

  // No threads/background
  // while (true)
  // {
    // for (auto io = maolan::IO::begin(); io != nullptr; io = io->next())
    // {
      // io->setup();
    // }
    // for (auto io = maolan::IO::begin(); io != nullptr; io = io->next())
    // {
      // io->fetch();
    // }
    // for (auto io = maolan::IO::begin(); io != nullptr; io = io->next())
    // {
      // io->process();
    // }
    // auto playhead = maolan::IO::playHead();
    // maolan::IO::playHead(playhead + maolan::Config::audioBufferSize);
  // }
  return 0;
}
