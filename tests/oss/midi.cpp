#include <fcntl.h>
#include <iostream>
#include <maolan/audio/oss/out.h>
#include <maolan/engine.h>
#include <maolan/midi/oss/in.h>


int main(int argc, char **argv)
{
  /* MIDI file input
   *
   */
  // if (argc < 2)
  // {
  // std::cerr << "Usage: " << argv[0] << " <midi file>" << std::endl;
  // return 1;
  // }
  // Config::bpms.emplace_back();
  // MIDIClip *clip = new MIDIClip();
  // clip->load(argv[1]);
  // MIDIChunk *chunk;
  // while ((chunk = clip->next()) != nullptr)
  // {
  //   std::cout << *chunk << std::endl;
  // }


  /* Beats
   * (60000ms / BPM) = Xms -> duration of 1/4 note or beat
  float bps = Config::bpm / 60.0;
  std::cout << Config::samplerate << ' ' << Config::bpm << std::endl;
  std::cout << Config::samplerate * 2 << ' ' << file.division << std::endl;
  float ratio = Config::samplerate / bps / file.division;
  int sample = 400;
  int res = ratio * sample;
  std::cout << ratio << ' ' << res << std::endl;
  */


  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <midi device>" << std::endl;
    return 1;
  }

  maolan::audio::OSSOut out("/dev/dsp", 2);
  maolan::midi::OSSMIDIIn midi(argv[1]);

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

  return 0;
}
