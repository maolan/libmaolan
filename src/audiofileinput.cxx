#include <maolan/audiofileinput>


using namespace std;


size_t AudioFileInput::size = 128;


AudioFileInput::AudioFileInput(const string &path)
  : audioFile(path)
{
  rawData = new float[size * audioFile.channels()];
  outputs.resize(audioFile.channels());
}


AudioFileInput::~AudioFileInput()
{
  delete []rawData;
}


void AudioFileInput::split()
{
  for (size_t channel = 0; channel < channels(); ++channel)
  {
    outputs[channel] = AudioChunk(new AudioChunkData(size));
    for (size_t i = 0; i < size; ++i)
    {
      outputs[channel]->data[i] = rawData[i * outputs.size() + channel];
    }
  }
}


void AudioFileInput::fetch()
{
  int bytesRead = audioFile.read(rawData, channels() * size);
  split();
}


size_t AudioFileInput::channels() const
{
  return audioFile.channels();
}
