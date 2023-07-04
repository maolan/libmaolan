#include "maolan/io.hpp"
#include "maolan/config.hpp"
#include "maolan/midi/input.hpp"
#include "maolan/midi/io.hpp"
#include <vector>


using namespace maolan::midi;


Buffer IO::pull(const std::size_t &ch)
{
  if (ch < _outputs.size())
  {
    return _outputs[ch];
  }
  return nullptr;
}


void IO::connect(IO *to)
{
  for (size_t channel = 0; channel < _inputs.size(); ++channel)
  {
    connect(to, channel, channel);
  }
}


void IO::connect(IO *to, std::size_t inCh, std::size_t outCh)
{
  _inputs[inCh]->connect(to, outCh);
}


std::size_t IO::channels() const { return _outputs.size(); }


nlohmann::json IO::connections()
{
  auto result = nlohmann::json::array();
  for (size_t i = 0; i < _inputs.size(); ++i)
  {
    auto &input = _inputs[i];
    auto injson = input->json(_name);
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


bool IO::leaf()
{
  std::size_t count = 0;
  for (const auto &input : _inputs)
  {
    count += input->conns();
  }
  return count == 0;
}


IO::IO(const std::string &name, const bool &reg) : maolan::IO(name, reg) {}
