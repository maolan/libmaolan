#include "maolan/audio/connectable.hpp"


using namespace maolan::audio;


std::vector<Connectable *> Connectable::_all;


nlohmann::json Connectable::json()
{
  auto data = nlohmann::json::array();
  for (auto &c : _all)
  {
    auto result = c->connections();
    if (result != nullptr) { data.push_back(result); }
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
  if (_inputs.size() == 0)
  {
    return nullptr;
  }
  auto result = nlohmann::json::array();
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

    nlohmann::json data;
    auto from = R"({})"_json;
    from["name"] = name;
    from["channel"] = i;
    data["from"] = from;
    data["to"] = to;
    result.push_back(data);
  }
  if (result.size() == 0)
  {
    return nullptr;
  }
  return result;
}
