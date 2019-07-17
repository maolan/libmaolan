#include <cstdint>
#include <iostream>
#include <maolan/audio/file.h>
#include <maolan/config.h>

using namespace maolan::audio;

File::File(const std::string &path, const uint64_t &offset)
    : IO(0, true, false), audioFile(path)
{
  audioFile.seek(offset, SEEK_SET);
  if (Config::audioBufferSize == 0)
  {
    std::cerr << "Loding order error. Load some hardware IO first!"
              << std::endl;
    exit(1);
  }
  _type = "File";
  outputs.resize(audioFile.channels(), nullptr);
  frame = new float[Config::audioBufferSize * channels()];
}


File::~File() { delete[] frame; }


void File::split()
{
  const auto chs = channels();
  for (size_t channel = 0; channel < channels(); ++channel)
  {
    outputs[channel] = Buffer(new BufferData(Config::audioBufferSize));
    for (size_t i = 0; i < Config::audioBufferSize; ++i)
    {
      outputs[channel]->data[i] = frame[i * chs + channel];
    }
  }
}


void File::fetch()
{
  int bytesRead = audioFile.read(frame, channels() * Config::audioBufferSize);
  split();
}


size_t File::channels() const { return audioFile.channels(); }


void File::process() {}


uint64_t File::offset() { return _offset; };

void File::offset(const uint64_t &argOffset) { _offset = argOffset; };
