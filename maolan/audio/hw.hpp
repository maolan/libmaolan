#pragma once
#include <string>
#include <vector>

#include "maolan/audio/io.hpp"


namespace maolan::audio
{
class HW : public IO
{
public:
  HW(const std::string &name, const std::string &device);
  virtual ~HW();

  static const std::vector<HW *> & all();

  int fd();

protected:
  static std::vector<HW *> _all;

  int _fd;
  std::string _device;
};
} // namespace maolan::audio
