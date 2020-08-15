#include <maolan/midi/connection.h>


using namespace maolan::midi;


Connection::Connection() {}


Connection::Connection(IO *connectTo) { target(connectTo); }


void Connection::target(IO *connectTo) { to = connectTo; }


Buffer Connection::pull() { return to->pull(); }


IO *Connection::get() { return to; }
