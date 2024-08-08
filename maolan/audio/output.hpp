#pragma once
#include <vector>

#include <maolan/audio/buffer.hpp>
#include <maolan/audio/connection.hpp>

namespace maolan::audio {
class Output {
public:
  Output();

  void connect(IO *to, const size_t &ch = 0);
  size_t conns();
  Buffer buffer();
  void buffer(Buffer b);

protected:
  std::vector<Connection *> _connections;
  Buffer _buffer;
};
} // namespace maolan::audio
