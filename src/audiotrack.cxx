#include <iostream>
#include <maolan/audiotrack>


using namespace std;


vector<AudioTrack *> AudioTrack::tracks;


AudioTrack::AudioTrack(const size_t &size)
  : AudioIO(size)
{
  name = "AudioTrack";
  trackIterator = tracks.emplace(tracks.end(), this);
}


AudioTrack::~AudioTrack()
{
  tracks.erase(trackIterator);
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


void AudioTrack::addFile(const string &filePath)
{
  files.push_back(AudioFileInput(filePath));
}
