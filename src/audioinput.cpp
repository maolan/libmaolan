#include <maolan/audioinput.h>
#include <maolan/config.h>

using namespace maolan::audio;

AudioInput::AudioInput()
  : AudioIO(1, true)
{
  _name = "AudioInput";
  connections.clear();
}


void AudioInput::add(AudioIO *to, size_t ch)
{
  auto it = connections.emplace(connections.end());
  it->target(to, ch);
}


void AudioInput::fetch()
{
  std::vector<AudioChunk> channels;
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
  if (empty) {
    outputs[0] = nullptr;
  }
  else if (channels.size() == 1)
  {
    outputs[0] = channels[0];
  }
  else
  {
    const AudioChunk result = AudioChunk(new AudioChunkData(Config::audioChunkSize));
    for (size_t i = 0; i < Config::audioChunkSize; ++i)
    {
      float sum = 0;
      for (auto channel : channels)
      {
        if (channel == nullptr) {continue;}
        if (channel->data[i] == 0.0) {continue;}
        sum += channel->data[i];
      }
      result->data[i] = sum;
    }
    outputs[0] = result;
  }
}


size_t AudioInput::channels() const
{
  return outputs.size();
}


AudioChunk AudioInput::pull()
{
  return outputs[0];
}


void AudioInput::process()
{
}
