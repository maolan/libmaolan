#include <maolan/config.h>
#include <maolan/midi/input.h>


using namespace maolan::midi;


Input::Input() : IO("MIDIInput")
{
  _type = "Input";
  connections.clear();
  outputs.resize(1);
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
    outputs[0] = nullptr;
  }
  else if (channels.size() == 1)
  {
    outputs[0] = channels[0];
  }
  else
  {
    outputs[0] = nullptr;
    Buffer lastBuffer;
    Buffer chunk;
    for (auto channel : channels)
    {
      if (channel == nullptr)
      {
        continue;
      }
      if (outputs[0] == nullptr)
      {
        chunk = std::make_shared<BufferData>();
        outputs[0] = chunk;
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


Buffer Input::pull()
{
  return outputs[0];
}


void Input::process() {}
