#include "maolan/audio/connection.hpp"


using namespace maolan::audio;


Connection::Connection(IO *connectTo, const std::size_t &ch)
{
  target(connectTo, ch);
}


void Connection::target(IO *connectTo, const std::size_t &ch)
{
  to = connectTo;
  _channel = ch;
}


Connection::Connection() {}
Buffer Connection::pull() { return to->pull(_channel); }
IO *Connection::get() { return to; }
std::size_t Connection::channel() { return _channel; }
void Connection::channel(const std::size_t &ch) { _channel = ch; }
