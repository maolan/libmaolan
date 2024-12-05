#pragma once
#include <maolan/audio/io.hpp>

namespace maolan::audio {
class Connection {
public:
  Connection(IO *connectTo, const size_t &ch);

  IO *to();
  void target(audio::IO *to, const size_t &ch);
  std::size_t channel();
  void channel(const size_t &ch);

protected:
  IO *_to;
  size_t _channel;
};
} // namespace maolan::audio
