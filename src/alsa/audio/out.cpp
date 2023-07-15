#include "maolan/alsa/audio/out.hpp"
#include "alsa/pcm.h"


using namespace maolan::audio;


template <typename T>
ALSAOut<T>::ALSAOut(const std::string &name, const std::string &device)
  : ALSA{name, device, SND_PCM_STREAM_PLAYBACK, sizeof(T)}
{}


namespace maolan::audio
{
template class ALSAOut<int32_t>;
template class ALSAOut<int16_t>;
template class ALSAOut<int8_t>;
} // namespace maolan::audio
