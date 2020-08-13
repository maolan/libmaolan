#include <iostream>
#include <maolan/midi/connectable.h>


using namespace maolan::midi;


void Connectable::connect(MIDIIO *to) { input.connect(to); }
void Connectable::fetch() { input.fetch(); }
void Connectable::process() { input.process(); }
