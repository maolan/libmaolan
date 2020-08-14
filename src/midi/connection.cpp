#include <maolan/midi/connection.h>


using namespace maolan::midi;


Connection::Connection() {}


Connection::Connection(MIDIIO *connectTo) { target(connectTo); }


void Connection::target(MIDIIO *connectTo) { to = connectTo; }


Buffer Connection::pull() { return to->pull(); }


MIDIIO *Connection::get() { return to; }
