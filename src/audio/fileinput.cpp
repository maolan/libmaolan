#include <iostream>
#include <maolan/audio/fileinput.h>
#include <maolan/config.h>
#include <cstdint>

using namespace maolan::audio;

FileInput::FileInput(const std::string &path)
  : IO(0, true, false)
  , audioFile(path)
{
  if (Config::audioChunkSize == 0)
  {
    std::cerr << "Loding order error. Load some hardware IO first!" << std::endl;
    exit(1); }
  _name = "FileInput";
  outputs.resize(audioFile.channels(), nullptr);
  rawData = new float[Config::audioChunkSize * channels()];

}


FileInput::~FileInput()
{
  delete []rawData;
}


void FileInput::split()
{
  const auto chs = channels();
  for (size_t channel = 0; channel < channels(); ++channel)
  {
    outputs[channel] = Chunk(new ChunkData(Config::audioChunkSize));
    for (size_t i = 0; i < Config::audioChunkSize; ++i)
    {
      outputs[channel]->data[i] = rawData[i * chs + channel];
    }
  }
}


void FileInput::fetch()
{
  int bytesRead = audioFile.read(rawData, channels() * Config::audioChunkSize);
  split();
}


size_t FileInput::channels() const
{
  return audioFile.channels();
}


void FileInput::process()
{
}
