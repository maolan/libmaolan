#include <iostream>
#include <vector>

#include "maolan/audio/input.hpp"
#include "maolan/audio/io.hpp"
#include "maolan/config.hpp"


using namespace maolan::audio;


std::vector<maolan::Config *> IO::devices;


IO::IO(const std::string &name, const bool &reg, const size_t &chs)
    : maolan::IO(name, reg)
{
  if (chs > 0)
  {
    _outputs.resize(chs);
  }
}


Buffer IO::pull(const std::size_t &channel)
{
  if (channels() > channel)
  {
    return _outputs[channel];
  }
  std::cerr << _type << ' ' << _name << " has " << channels();
  std::cerr << " channels and channel " << channel + 1 << " requested!\n";
  return nullptr;
}


nlohmann::json IO::json()
{
  auto result = maolan::IO::json();
  result["channels"] = channels();
  return result;
}


void IO::connect(IO *to)
{
  for (std::size_t channel = 0; channel < _inputs.size(); ++channel)
  {
    connect(to, channel, channel);
  }
}


void IO::connect(IO *to, std::size_t inCh, std::size_t outCh)
{
  _inputs[inCh]->connect(to, outCh);
}


void IO::fetch()
{
  for (auto &input : _inputs)
  {
    input->fetch();
  }
}


void IO::process()
{
  for (auto &input : _inputs)
  {
    input->process();
  }
}


std::size_t IO::channels() const { return _outputs.size(); }
