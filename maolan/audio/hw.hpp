#pragma once
#include <string>
#include <vector>

#include <maolan/audio/io.hpp>

namespace maolan::audio {
class HW : public IO {
public:
  HW(const std::string &name, const std::string &device, const bool &direction);
  virtual ~HW();

  static const std::vector<HW *> &all();

  int fd();
  std::string device();
  bool direction();

protected:
  static std::vector<HW *> _all;

  int _fd;
  std::string _device;
  bool _direction;
};

typedef HW *(*audio_t)(const std::string &name, const std::string &device);
typedef std::vector<HW *> *(*list_t)();
} // namespace maolan::audio
