#include <iostream>
#include <maolan/midi/connectable.h>


using namespace maolan::midi;


void Connectable::connect(IO *to) { input.connect(to); }
void Connectable::fetch() { input.fetch(); }
void Connectable::process() { input.process(); }
