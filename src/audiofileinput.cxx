#include <maolan/audiofileinput>
#include <maolan/config>


using namespace std;


AudioFileInput::AudioFileInput(const string &path)
  : audioFile(path)
{
  name = "AudioFileInput";
  rawData = new float[Config::audioChunkSize * audioFile.channels()];
  outputs.resize(audioFile.channels(), nullptr);
}


AudioFileInput::~AudioFileInput()
{
  delete []rawData;
}


void AudioFileInput::split()
{
  for (size_t channel = 0; channel < channels(); ++channel)
  {
    outputs[channel] = AudioChunk(new AudioChunkData(Config::audioChunkSize));
    for (size_t i = 0; i < Config::audioChunkSize; ++i)
    {
      outputs[channel]->data[i] = rawData[i * outputs.size() + channel];
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
