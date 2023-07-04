#pragma once
#include <string>
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
  virtual nlohmann::json json(const std::string &name, const size_t &channel);
  std::size_t conns();

protected:
  std::vector<audio::Connection *> _connections;
  Buffer _output;
};
} // namespace maolan::audio
