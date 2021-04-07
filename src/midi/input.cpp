#include "maolan/config.hpp"
#include "maolan/midi/input.hpp"


using namespace maolan::midi;


Input::Input() : IO("MIDIInput")
{
  _type = "MIDIInput";
  connections.clear();
  _outputs.resize(1);
}


void Input::connect(IO *to, const std::size_t &ch)
{
  auto it = connections.emplace(connections.end());
  it->target(to, ch);
}


void Input::fetch()
{
  std::vector<Buffer> channels(connections.size());
  bool empty = true;
  for (std::size_t i = 0; i < connections.size(); ++i)
  {
    const auto element = connections[i].pull();
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
    _outputs[0] = nullptr;
    Buffer lastBuffer;
    Buffer chunk;
    for (auto channel : channels)
    {
      if (channel == nullptr)
      {
        continue;
      }
      if (_outputs[0] == nullptr)
      {
        chunk = std::make_shared<BufferData>();
        _outputs[0] = chunk;
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


Buffer Input::pull(const std::size_t &channel)
{
  return _outputs[channel];
}


void Input::process() {}
Input::~Input() {}
