#include <maolan/frame.h>


using namespace maolan;


Frame::Frame(std::size_t audioSize, std::size_t controlSize)
{
  if (audioSize > 0) { audioBuffer.resize(audioSize); }
  if (controlSize > 0) { controls = new float[controlSize]; }
  else { controls = nullptr; }
}


Frame::~Frame()
{
  audioBuffer.resize(0);
  if (controls != nullptr) { delete []controls; }
}
