#include <maolan/audio/clip.h>
#include <sndfile.hh>
#include <maolan/audio/file.h>
#include <iostream>

using namespace maolan::audio;


Clip::Clip(const uint64_t &offset, const std::string &path)
  : IO(0, true)
  , _offset{offset}
  , file(path)
{
  _name = "Clip";
  connect(&file);
}

void Clip::fetch()
{
  file.fetch();
  file.process();
}

void Clip::process()
{
}

std::size_t Clip::channels() const
{
  return file.channels();
}

uint64_t Clip::offset()
{
  return _offset;
}


Chunk Clip::pull(const unsigned &channel)
{
  return file.pull(channel);
}
