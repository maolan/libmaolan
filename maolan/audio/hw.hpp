#pragma once
#include <vector>

#include "maolan/audio/io.hpp"


namespace maolan::audio
{
class HW : public IO
{
public:
  HW(const std::string &name);
  virtual ~HW();

  static const std::vector<HW *> & all();

  int fd();

protected:
  static std::vector<HW *> _all;

  int _fd;
};
} // namespace maolan
