#include <iostream>
#include <vector>

#include "maolan/audio/input.hpp"
#include "maolan/audio/io.hpp"
#include "maolan/config.hpp"


using namespace maolan::audio;


IO::IO(const std::string &name, const bool &reg, const size_t &chs)
    : maolan::IO(name, reg)
{
  if (chs > 0)
  {
    _outputs.resize(chs);
  }
  for (size_t i = 0; i < chs; ++i)
  {
    auto input = new Input();
    _inputs.push_back(input);
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


nlohmann::json IO::connections()
{
  auto result = nlohmann::json::array();
  for (size_t i = 0; i < _inputs.size(); ++i)
  {
    auto &input = _inputs[i];
    auto injson = input->json(_name, i);
    if (injson != nullptr)
    {
      result.push_back(injson);
    }
  }
  if (result.size() == 0)
  {
    return nullptr;
  }
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
  if (inCh < _inputs.size())
  {
    std::cout << "Connecting " << _name << " and " << to->name() << '\n';
    _inputs[inCh]->connect(to, outCh);
  }
}


void IO::fetch()
{
  for (auto &input : _inputs)
  {
    input->fetch();
    input->pull();
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
