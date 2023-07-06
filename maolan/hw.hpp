#pragma once
#include <atomic>


namespace maolan
{
class HW
{
public:
  HW(const bool &audio = false);
  virtual ~HW();

protected:
  static std::atomic_size_t _size;
  static std::atomic_size_t _audio_size;

  bool _audio;
};
} // namespace maolan
