#include <iostream>
#include <cstdint>
#include <maolan/audio/oss/out.hpp>
#include <maolan/engine.hpp>
#include <maolan/midi/oss/in.hpp>
#include <maolan/plugin/instrument.hpp>


namespace lv2 = maolan::plugin::lv2;
namespace midi = maolan::midi;
namespace audio = maolan::audio;


int main(int argc, char **argv)
{
  audio::OSSOut<int32_t> out("/dev/dsp");
  midi::OSSMIDIIn midiIn("/dev/umidi0.0");
  auto drumgizmo = new maolan::plugin::Instrument("http://drumgizmo.org/lv2", "lv2");
  drumgizmo->connect(&midiIn);
  out.connect(drumgizmo->audio());

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

  delete drumgizmo;
  return 0;
}
