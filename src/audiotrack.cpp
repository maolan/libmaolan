#include <maolan/audiotrack.h>

using namespace maolan::audio;

Track::Track(const size_t &size)
  : IO(size)
{
  _name = "Track";
}


void Track::fetch()
{
  for (auto &file : files)
  {
    file.fetch();
  }
}


void Track::process()
{
  for (size_t i = 0; i < channels(); ++i)
  {
    outputs[i] = files[0].pull(i);
  }
}


size_t Track::channels() const
{
  return outputs.size();
}


void Track::connect(IO *to)
{
  for (size_t channel = 0; channel < channels(); ++channel)
  {
    inputs[channel].add(to, channel);
  }
}


void Track::addFile(const std::string &filePath)
{
  files.push_back(FileInput(filePath));
}
