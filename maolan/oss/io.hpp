#pragma once
#include <vector>

#include "maolan/audio/io.hpp"
#include "maolan/hw.hpp"


namespace maolan::audio::oss
{
class IO : public maolan::audio::IO, public maolan::HW
{
public:
  IO(const std::string &device, const bool &audio);
  ~IO();

  static maolan::IO *wait();

protected:
  static std::vector<IO *> _all;
};
} // namespace maolan::audio::oss
