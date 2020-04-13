#include <iostream>
#include <maolan/config.h>
#include <maolan/audio/clip.h>
#include <maolan/audio/track.h>


using namespace maolan::audio;


Clip::Clip(Track *parent, const std::size_t &ch)
  : IO(0, true, false)
  , file(ch)
{
  _type = "Clip";
}


Clip::Clip(
  const std::string &path,
  const uint64_t &start,
  const uint64_t &end,
  const uint64_t &offset,
  Track *parent
)
  : IO(0, true, false)
  , _offset(offset)
  , _start(start)
  , _end(end)
  , _previous(nullptr)
  , _next(nullptr)
  , file(path, offset)
  , _parent(parent)
{
  _type = "Clip";
  parent->add(this);
}


Clip::~Clip()
{
  if (_parent != nullptr) { _parent->remove(this); }
}


void Clip::fetch()
{
  if (_playHead >= _start)
  {
    file.fetch();
    file.process();
  }
}


Buffer Clip::pull(const unsigned &channel)
{
  if (_playHead >= _start)
  {
    return file.pull(channel);
  }
  return nullptr;
}


void Clip::parent(maolan::IO *p)
{
  if (_parent != nullptr) { _parent->remove(this); }
  _parent = (Track *)p;
  if (_parent != nullptr) { _parent->add(this); }
}


void Clip::write(const Frame &fr)
{
  file.write(fr);
  _end += Config::audioBufferSize;
}


void Clip::write(const Frame * const fr) { write(*fr); }
void Clip::next(Clip *n) { _next = n; }
Clip *Clip::next() { return _next; }
void Clip::previous(Clip *p) { _previous = p; }
Clip *Clip::previous() { return _previous; }
void Clip::process() {}
std::size_t Clip::channels() const { return file.channels(); }
uint64_t Clip::offset() { return _offset; }
void Clip::offset(const uint64_t &argOffset) { _offset = argOffset; }
uint64_t Clip::start() { return _start; }
void Clip::start(const uint64_t &argStart) { _start = argStart; }
uint64_t Clip::end() { return _end; }
void Clip::end(const uint64_t &argEnd) { _end = argEnd; }
bool Clip::check() { return file.audioFile().error() == 0; }
