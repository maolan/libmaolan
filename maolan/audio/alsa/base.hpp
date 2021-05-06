#pragma once
#include "maolan/audio/alsa/config.hpp"
#include "maolan/audio/io.hpp"


namespace maolan::audio
{
class ALSA : public IO
{
public:
protected:
  float *_frame;
  ALSAConfig *_device;
};
} // namespace maolan::audio
