#include <cstdint>
#include <iostream>
#include "maolan/audio/file.hpp"
#include "maolan/config.hpp"


using namespace maolan::audio;


File::File(const std::size_t &ch) : IO(0, true, false), recording(true)
{
  std::string path = "recording.wav";
  _audioFile = SndfileHandle(path, SFM_RDWR, SF_FORMAT_WAV | SF_FORMAT_FLOAT,
                             ch, Config::samplerate);
  _type = "File";
  _name = path;
  const auto chs = _audioFile.channels();
  outputs.resize(chs, nullptr);
  frame = new float[Config::audioBufferSize * chs];
}


File::File(const std::string &path, const uint64_t &offset)
    : IO(0, true, false, path), recording(false)
{
  _audioFile = SndfileHandle(path);
  if (_audioFile.error())
  {
    std::cerr << "_audiofile error " << _audioFile.strError() << std::endl;
    exit(1);
  }
  if (offset)
  {
    _audioFile.seek(offset, SEEK_SET);
  }
  if (Config::audioBufferSize == 0)
  {
    std::cerr << "Loading order error. Load some hardware IO first!\n";
    exit(1);
  }
  _type = "File";
  const auto chs = _audioFile.channels();
  outputs.resize(chs, nullptr);
  frame = new float[Config::audioBufferSize * chs];
}


void File::split()
{
  if (frame == nullptr)
  {
    return;
  }
  const auto chs = channels();
  for (std::size_t channel = 0; channel < chs; ++channel)
  {
    outputs[channel] = std::make_shared<BufferData>(Config::audioBufferSize);
    for (std::size_t i = 0; i < Config::audioBufferSize; ++i)
    {
      outputs[channel]->data()[i] = frame[i * chs + channel];
    }
  }
}


void File::fetch()
{
  if (!recording)
  {
    auto size = channels() * Config::audioBufferSize;
    int bytesRead = _audioFile.read(frame, size);
    split();
  }
}


void File::write(const Frame &fr)
{
  const auto chs = fr.audio.size();
  std::size_t i;
  std::size_t channel;
  float sample;
  for (channel = 0; channel < chs; ++channel)
  {
    for (i = 0; i < Config::audioBufferSize; ++i)
    {
      auto &buffer = fr.audio[channel];
      if (buffer)
      {
        auto data = buffer->data();
        sample = data[i];
      }
      else
      {
        sample = 0.0;
      }
      frame[i * chs + channel] = sample;
    }
  }
  int bytesWritten = _audioFile.writef(frame, Config::audioBufferSize);
}


File::~File() { delete[] frame; }
void File::write(const Frame *const fr) { write(*fr); }
std::size_t File::channels() const { return _audioFile.channels(); }
void File::process() {}
uint64_t File::offset() { return _offset; }
void File::offset(const uint64_t &argOffset) { _offset = argOffset; }
SndfileHandle File::audioFile() { return _audioFile; }
