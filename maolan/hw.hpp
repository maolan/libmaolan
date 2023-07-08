#pragma once
#include <atomic>

#include "maolan/io.hpp"


struct pollfd;

namespace maolan
{
class HW
{
public:
  HW(const bool &audio = false);
  virtual ~HW();

  static IO * wait();

  virtual struct pollfd * pollfd() = 0;

protected:
  static std::atomic_size_t _size;
  static std::atomic_size_t _audio_size;

  bool _audio;
  int _fd;
};
} // namespace maolan
