#pragma once
#include <vector>

#include "maolan/audio/io.hpp"

struct kevent;

namespace maolan::audio::oss
{
class IO : public maolan::audio::IO
{
public:
  IO(const std::string &device);
  ~IO();

  static maolan::IO *wait();

protected:
  static std::vector<IO *> ios;
  static int kq;
  struct kevent *event;
};
} // namespace maolan::audio::oss
