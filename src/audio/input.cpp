#include "maolan/audio/input.hpp"
#include "maolan/config.hpp"


using namespace maolan::audio;


void Input::connect(audio::IO *to, const std::size_t &ch)
{
  _connections.push_back(new Connection(to, ch));
}


void Input::fetch()
{
  std::vector<Buffer> channels{_connections.size()};
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
    const auto result = std::make_shared<BufferData>(Config::audioBufferSize);
    for (size_t i = 0; i < Config::audioBufferSize; ++i)
    {
      float sum = 0;
      for (auto channel : channels)
      {
        if (channel == nullptr)
        {
          continue;
        }
        sum += channel->data()[i];
      }
      result->data()[i] = sum;
    }
    _output = result;
  }
}


nlohmann::json Input::json(const std::string &name, const size_t &channel)
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
    data["channel"] = connection->channel();
    result.push_back(data);
  }
  if (result.size() > 0)
  {
    auto data = R"({})"_json;
    data["name"] = name;
    data["channel"] = channel;
    data["to"] = result;
    return data;
  }
  return nullptr;
}


Buffer Input::pull() { return _output; }
void Input::process() {}
std::size_t Input::conns() { return _connections.size(); }
