#include <maolan/frame.h>


using namespace maolan;


Frame::Frame(const std::size_t &audioSize, const std::size_t &midiSize,
             const std::size_t &controlSize)
{
  if (audioSize > 0)
  {
    audio.resize(audioSize);
  }
  if (midiSize > 0)
  {
    midi.resize(midiSize);
  }
  if (controlSize > 0)
  {
    control.resize(controlSize);
  }
}
