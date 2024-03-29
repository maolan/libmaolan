#pragma once
#include <string>
#include <vector>

#include <maolan/midi/io.hpp>

namespace maolan::midi {
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

typedef HW *(*midi_t)(const std::string &name, const std::string &device);
typedef std::vector<HW *> *(*list_t)();
} // namespace maolan::midi
