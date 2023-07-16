#pragma once
#include <string>

#include "maolan/alsa/audio/base.hpp"


namespace maolan::audio
{
template <typename T> class ALSAOut : public ALSA
{
public:
  ALSAOut(const std::string &name, const std::string &device);

  virtual void fetch();
  virtual void process();
  virtual void writehw();
};
}
