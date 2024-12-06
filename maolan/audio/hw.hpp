#pragma once
#include <string>
#include <vector>

#include <maolan/audio/io.hpp>

namespace maolan::audio {
class HW : public IO {
public:
  HW(const std::string &name, const std::string &device);
  virtual ~HW();

  static const std::vector<HW *> &all();

  int fd();
  std::string device();

protected:
  static std::vector<HW *> _all;

  int _fd;
  std::string _device;
};

typedef HW *(*audio_t)(const std::string &name, const std::string &device);
typedef std::vector<HW *> *(*list_t)();
} // namespace maolan::audio
