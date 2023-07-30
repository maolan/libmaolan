#pragma once
#include <vector>

#include <maolan/midi/buffer.hpp>
#include <maolan/midi/io.hpp>

namespace maolan::midi {
class Input {
public:
  void fetch();
  void process();
  void connect(IO *to);
  Buffer pull();
  virtual nlohmann::json json(const std::string &name);
  std::size_t conns();

protected:
  std::vector<midi::IO *> _connections;
  Buffer _output;
};
} // namespace maolan::midi
