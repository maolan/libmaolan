#include <iostream>
#include <maolan/audiofileinput>
#include <maolan/config>


using namespace std;


AudioFileInput::AudioFileInput(const string &path)
  : audioFile(path)
{
  name = "AudioFileInput";
  outputs.resize(audioFile.channels(), nullptr);
  cout << "AudioFileInput::AudioFileInput::Config::audioChunkSize = " << Config::audioChunkSize << endl;
  rawData = new float[Config::audioChunkSize * channels()];
}


AudioFileInput::~AudioFileInput()
{
  delete []rawData;
}


void AudioFileInput::split()
{
  const auto chs = channels();
  for (size_t channel = 0; channel < channels(); ++channel)
  {
    outputs[channel] = AudioChunk(new AudioChunkData(Config::audioChunkSize));
    for (size_t i = 0; i < Config::audioChunkSize; ++i)
    {
      outputs[channel]->data[i] = rawData[i * chs + channel];
    }
  }
}


void AudioFileInput::fetch()
{
  int bytesRead = audioFile.read(rawData, channels() * Config::audioChunkSize);
  split();
}


size_t AudioFileInput::channels() const
{
  return audioFile.channels();
}
