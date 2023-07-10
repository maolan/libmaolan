#pragma once
#include <vector>

#include "maolan/midi/io.hpp"


namespace maolan::midi
{
class HW : public IO
{
public:
  HW(const std::string &name);
  virtual ~HW();

  static const std::vector<HW *> &all();

  int fd();

protected:
  static std::vector<HW *> _all;

  int _fd;
};
} // namespace maolan::midi
