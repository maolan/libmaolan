#include <maolan/config.h>
#include <maolan/midi/io.h>
#include <vector>


using namespace maolan::midi;


std::vector<maolan::Config *> IO::devices;


IO::IO(const std::string &name, const bool &front, const bool &reg)
    : maolan::IO(name, front, reg)
{
}


Buffer IO::pull() { return nullptr; }
