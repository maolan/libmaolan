#include "maolan/audio/input.hpp"
#include "maolan/config.hpp"


using namespace maolan::audio;


void Input::connect(audio::IO *to, const std::size_t &ch)
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
    _outputs[0] = nullptr;
  }
  else if (channels.size() == 1)
  {
    _outputs[0] = channels[0];
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
    _outputs[0] = result;
  }
}


size_t Input::channels() const { return _outputs.size(); }


Buffer Input::pull(const std::size_t &channel)
{
  if (_outputs.size() == 0)
  {
    return nullptr;
  }
  return _outputs[channel];
}


nlohmann::json Input::connections()
{
  if (_connections.size() == 0) { return nullptr; }
  auto data = R"([])"_json;
  for (auto &con: _connections)
  {
    auto to = R"({})"_json;
    to["name"] = con->get()->name();
    to["channel"] = con->channel();
    data.push_back(to);
  }
  return data;
}


Input::Input() : IO(1, true) { _type = "AudioInput"; }
Input::~Input() {}
void Input::process() {}
