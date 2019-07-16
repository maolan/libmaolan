#include <maolan/audio/clip.h>
#include <maolan/audio/file.h>
#include <sndfile.hh>

using namespace maolan::audio;


Clip::Clip(const uint64_t &start, const uint64_t &offset,
            const std::string &path)
    : IO(0, true), _offset{offset}, file(path)
{
  _type = "Clip";
  connect(&file);
}

void Clip::fetch()
{
  file.fetch();
  file.process();
}

void Clip::process() {}

std::size_t Clip::channels() const { return file.channels(); }

uint64_t Clip::offset() { return _offset; }

void Clip::offset(const uint64_t &offset) { _offset = offset; }

uint64_t Clip::start() { return _start; }

void Clip::start(const uint64_t &start) { _start = start; }

Buffer Clip::pull(const unsigned &channel) { return file.pull(channel); }
