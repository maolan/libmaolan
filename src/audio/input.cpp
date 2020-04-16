#include <iostream>
#include <maolan/audio/input.h>
#include <maolan/config.h>


using namespace maolan::audio;


Input::Input() : IO(1, true)
{
  _type = "Input";
  connections.clear();
}


void Input::connect(IO *to, const std::size_t &ch)
{
  auto it = connections.emplace(connections.end());
  it->target(to, ch);
}


void Input::fetch()
{
  std::vector<Buffer> channels;
  bool empty = true;
  for (auto &connection : connections)
  {
    const auto element = connection.pull();
    if (element != nullptr)
    {
      empty = false;
      channels.push_back(element);
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
    const Buffer result = Buffer(new BufferData(Config::audioBufferSize));
    for (size_t i = 0; i < Config::audioBufferSize; ++i)
    {
      float sum = 0;
      for (auto channel : channels)
      {
        if (channel == nullptr)
        {
          continue;
        }
        sum += channel->data[i];
      }
      result->data[i] = sum;
    }
    outputs[0] = result;
  }
}


size_t Input::channels() const { return outputs.size(); }


Buffer Input::pull() {
  if (outputs.size() == 0) { return nullptr; }
  return outputs[0];
}


void Input::process() {}
