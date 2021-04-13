#pragma once
#include <vector>

#include "maolan/audio/buffer.hpp"
#include "maolan/audio/connection.hpp"


namespace maolan::audio
{
class Input
{
public:
  void fetch();
  void process();
  void connect(IO *to, const std::size_t &ch = 0);
  Buffer pull();

protected:
  std::vector<audio::Connection *> _connections;
  Buffer _output;
};
} // namespace maolan::audio
