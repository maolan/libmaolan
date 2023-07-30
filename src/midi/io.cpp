#include <maolan/config.hpp>
#include <maolan/io.hpp>
#include <maolan/midi/input.hpp>
#include <maolan/midi/io.hpp>

using namespace maolan::midi;

IO::IO(const std::string &name, const bool &reg)
    : maolan::IO{name, reg}, _output{nullptr}, _input{new Input()} {}

void IO::connect(IO *to) { _input->connect(to); }
nlohmann::json IO::connections() { return _input->json(_name); }
void IO::fetch() { _input->fetch(); }
void IO::process() { _input->process(); }
Buffer IO::pull() { return _output; }
