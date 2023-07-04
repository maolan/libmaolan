#pragma once
#include <vector>

#include "maolan/midi/buffer.hpp"
#include "maolan/midi/connection.hpp"


namespace maolan::midi
{
class Input
{
public:
  void fetch();
  void process();
  void connect(IO *to, const std::size_t &ch = 0);
  Buffer pull();
  virtual nlohmann::json json(const std::string &name);
  std::size_t conns();

protected:
  std::vector<midi::Connection *> _connections;
  Buffer _output;
};
} // namespace maolan::midi
