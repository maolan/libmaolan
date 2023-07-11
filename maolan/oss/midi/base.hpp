#pragma once
#include <string>

#include "maolan/midi/buffer.hpp"
#include "maolan/midi/hw.hpp"
#include "maolan/midi/io.hpp"


namespace maolan::midi
{
class OSS : public maolan::midi::HW
{
public:
  OSS(const std::string &name, const std::string &device);
  ~OSS();

protected:
  Buffer _data;
};
} // namespace maolan::midi
