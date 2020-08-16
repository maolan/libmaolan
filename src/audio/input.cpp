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
    outputs[0] = result;
  }
}


size_t Input::channels() const { return outputs.size(); }


Buffer Input::pull()
{
  if (outputs.size() == 0)
  {
    return nullptr;
  }
  return outputs[0];
}


void Input::process() {}
