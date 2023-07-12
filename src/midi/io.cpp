#include "maolan/io.hpp"
#include "maolan/config.hpp"
#include "maolan/midi/input.hpp"
#include "maolan/midi/io.hpp"
#include <vector>


using namespace maolan::midi;


void IO::connect(IO *to)
{
  _input->connect(to);
}


nlohmann::json IO::connections()
{
  return _input->json(_name);
}


IO::IO(const std::string &name, const bool &reg)
  : maolan::IO{name, reg}
  , _output{nullptr}
  , _input{new Input()}
{}


Buffer IO::pull() { return _output; }
