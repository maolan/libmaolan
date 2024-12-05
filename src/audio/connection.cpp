#include <maolan/audio/connection.hpp>

using namespace maolan::audio;

void Connection::target(IO *to, const size_t &ch) {
  _to = to;
  _channel = ch;
}

Connection::Connection(IO *to, const size_t &ch) { target(to, ch); }
IO *Connection::to() { return _to; }
std::size_t Connection::channel() { return _channel; }
void Connection::channel(const size_t &ch) { _channel = ch; }
