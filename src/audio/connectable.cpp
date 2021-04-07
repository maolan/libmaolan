#include <iostream>
#include "maolan/audio/connectable.hpp"


using namespace maolan::audio;


std::vector<Connectable *> Connectable::_all;


nlohmann::json Connectable::json()
{
  auto data = nlohmann::json::array();
  for (auto &c : _all)
  {
    auto result = c->connections();
    if (result != nullptr) 
    { 
      for (auto &r : result) { data.push_back(r); }
    }
  }
  if (data.size() == 0) { return nullptr; }
  return data;
}


Connectable::Connectable(const std::size_t &chs) 
{ 
  _inputs.resize(chs); 
  _all.push_back(this);
}


void Connectable::connect(IO *to)
{
  for (std::size_t channel = 0; channel < _inputs.size(); ++channel)
  {
    connect(to, channel, channel);
  }
}


void Connectable::connect(IO *to, std::size_t inCh, std::size_t outCh)
{
  _inputs[inCh].connect(to, outCh);
}


void Connectable::fetch()
{
  for (auto &input : _inputs)
  {
    input.fetch();
  }
}


void Connectable::process()
{
  for (auto &input : _inputs)
  {
    input.process();
  }
}


nlohmann::json Connectable::conns(const std::string_view name)
{
  if (_inputs.size() == 0) { return nullptr; }
  bool empty = true;
  auto result = R"([])"_json;
  for (std::size_t i = 0; i < _inputs.size(); ++i)
  {
    auto &input = _inputs[i];
    if (input.channels() == 0)
    {
      continue;
    }
    auto to = input.connections();
    if (to == nullptr)
    {
      continue;
    }

    empty = false;
    nlohmann::json data;
    data["channel"] = i;
    data["to"] = to;
    data["name"] = name.data();
    result.push_back(data);
  }
  if (empty) { return nullptr; }
  return result;
}
