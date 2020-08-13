#include <maolan/config.h>
#include <maolan/midi/input.h>


using namespace maolan::midi;


Input::Input() : MIDIIO("MIDIInput")
{
  _type = "Input";
  connections.clear();
}


void Input::connect(MIDIIO *to)
{
  auto it = connections.emplace(connections.end());
  it->target(to);
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
    output = nullptr;
  }
  else if (channels.size() == 1)
  {
    output = channels[0];
  }
  else
  {
    output = nullptr;
    BufferData *lastBuffer = nullptr;
    BufferData *chunk = nullptr;
    for (auto channel : channels)
    {
      if (channel == nullptr)
      {
        continue;
      }
      if (output == nullptr) {
        output = std::make_shared<BufferData>();
        chunk = output.get();
      } else {
        chunk = new BufferData;
      }
      *chunk = *channel;
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
  return output;
}


void Input::process()
{}
