#pragma once
#include <string>

#include "maolan/sndio/audio/base.hpp"


namespace maolan::audio
{
template <typename T> class SNDIOIn : public SNDIO
{
public:
  SNDIOIn(const std::string &name, const std::string &device);

  virtual void readhw();
  virtual void process();
};
} // namespace maolan::audio

