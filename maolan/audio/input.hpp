#pragma once
#include <string>
#include <vector>

#include <maolan/audio/buffer.hpp>
#include <maolan/audio/connection.hpp>

namespace maolan::audio {
class Input {
public:
  void fetch();
  void process();
  void connect(IO *to, const size_t &ch = 0);
  Buffer pull();
  virtual nlohmann::json json(const std::string &name, const size_t &channel);
  size_t conns();
  std::vector<Connection *> connections();

protected:
  std::vector<Connection *> _connections;
  Buffer _output;
};
} // namespace maolan::audio
