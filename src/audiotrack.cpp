#include <maolan/audiotrack.h>


AudioTrack::AudioTrack(const size_t &size)
  : AudioIO(size)
{
  _name = "AudioTrack";
}


void AudioTrack::fetch()
{
  for (auto &file : files)
  {
    file.fetch();
  }
}


void AudioTrack::process()
{
  for (size_t i = 0; i < channels(); ++i)
  {
    outputs[i] = files[0].pull(i);
  }
}


size_t AudioTrack::channels() const
{
  return outputs.size();
}


void AudioTrack::connect(AudioIO *to)
{
  for (size_t channel = 0; channel < channels(); ++channel)
  {
    inputs[channel].add(to, channel);
  }
}


void AudioTrack::addFile(const std::string &filePath)
{
  files.push_back(AudioFileInput(filePath));
}
