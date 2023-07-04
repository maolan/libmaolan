#include "maolan/midi/input.hpp"
#include "maolan/config.hpp"
#include <iostream>


using namespace maolan::midi;


void Input::connect(IO *to, const std::size_t &ch)
{
  _connections.push_back(new Connection(to, ch));
}


void Input::fetch()
{
  std::vector<Buffer> channels(_connections.size());
  bool empty = true;
  for (std::size_t i = 0; i < _connections.size(); ++i)
  {
    const auto element = _connections[i]->pull();
    channels[i] = element;
    if (element != nullptr)
    {
      empty = false;
    }
  }
  if (empty)
  {
    _output = nullptr;
  }
  else if (channels.size() == 1)
  {
    _output = channels[0];
  }
  else
  {
    _output = nullptr;
    Buffer lastBuffer;
    Buffer chunk;
    for (auto channel : channels)
    {
      if (channel == nullptr)
      {
        continue;
      }
      if (_output == nullptr)
      {
        chunk = std::make_shared<BufferData>();
        _output = chunk;
      }
      else
      {
        chunk = std::make_shared<BufferData>();
      }
      *chunk = *channel;
      chunk->next = nullptr;
      if (lastBuffer != nullptr)
      {
        lastBuffer->next = chunk;
      }
      lastBuffer = chunk;
    }
  }
}


nlohmann::json Input::json(const std::string &name)
{
  auto result = R"([])"_json;
  for (auto connection : _connections)
  {
    if (connection == nullptr)
    {
      continue;
    }
    auto data = R"({})"_json;
    data["name"] = connection->get()->name();
    result.push_back(data);
  }
  if (result.size() > 0)
  {
    auto data = R"({})"_json;
    data["name"] = name;
    data["to"] = result;
    return data;
  }
  return nullptr;
  return nullptr;
}


Buffer Input::pull() { return _output; }
void Input::process() {}
std::size_t Input::conns() { return _connections.size(); }
