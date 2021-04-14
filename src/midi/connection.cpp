#include "maolan/midi/connection.hpp"


using namespace maolan::midi;


Connection::Connection(IO *connectTo, const std::size_t &ch)
{
  target(connectTo, ch);
}


void Connection::target(IO *connectTo, const std::size_t &ch)
{
  to = connectTo;
  _channel = ch;
}


Buffer Connection::pull() { return to->pull(_channel); }


IO *Connection::get() { return to; }
