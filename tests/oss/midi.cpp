#include <chrono>
#include <cstdint>
#include <iostream>
#include <maolan/engine.hpp>
#include <maolan/midi/clip.hpp>
#include <maolan/midi/track.hpp>
#include <maolan/oss/audio/out.hpp>
#include <maolan/oss/midi/in.hpp>
#include <maolan/oss/midi/out.hpp>
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

  audio::OSSOut<int32_t> out("/dev/dsp");
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
  //   for (auto io = maolan::IO::begin(); io != nullptr; io = io->next())
  //   {
  //     io->setup();
  //   }
  //   for (auto io = maolan::IO::begin(); io != nullptr; io = io->next())
  //   {
  //     io->fetch();
  //   }
  //   for (auto io = maolan::IO::begin(); io != nullptr; io = io->next())
  //   {
  //     io->process();
  //   }
  //   auto playhead = maolan::IO::playHead();
  //   maolan::IO::playHead(playhead + maolan::Config::audioBufferSize);
  // }
  return 0;
}
