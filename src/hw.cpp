#include "maolan/hw.hpp"


using namespace maolan;


std::atomic_size_t HW::_size = 0;
std::atomic_size_t HW::_audio_size = 0;


HW::HW(const bool &audio) : _audio{audio}
{
  ++_size;
  if (_audio)
  {
    ++_audio_size;
  }
}


HW::~HW()
{
  --_size;
  if (_audio)
  {
    --_audio_size;
  }
}
