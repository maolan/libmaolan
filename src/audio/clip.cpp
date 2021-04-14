#include <iostream>

#include "maolan/audio/clip.hpp"
#include "maolan/audio/track.hpp"
#include "maolan/config.hpp"


using namespace maolan::audio;


Clip::Clip(const std::string &path, Track *parent, const std::size_t &start,
           const std::size_t &end, const std::size_t &offset)
    : IO{path, false, 0}
    , _offset{offset}
    , _start{start}
    , _end{end}
    , file{path, offset}
    , _parent{parent}
    , _next{nullptr}
    , _previous{nullptr}
{
  _type = "AudioClip";
  if (parent) { parent->add(this); }
}


Clip::~Clip()
{
  if (_parent) { _parent->remove(this); }
}


Buffer Clip::pull(const std::size_t &channel)
{
  if (_playHead >= _start) { return file.pull(channel); }
  return nullptr;
}


void Clip::parent(maolan::IO *p)
{
  if (_parent) { _parent->remove(this); }
  _parent = (Track *)p;
  if (_parent) { _parent->add(this); }
}


void Clip::write(const Frame &fr)
{
  file.write(fr);
  _end += Config::audioBufferSize;
}


nlohmann::json Clip::json()
{
  auto data = IO::json();
  data["offset"] = _offset;
  data["start"] = _start;
  data["end"] = _end;
  return data;
}


void Clip::init() { file.init(); }
void Clip::write(const Frame *const fr) { write(*fr); }
void Clip::next(Clip *n) { _next = n; }
Clip *Clip::next() { return _next; }
void Clip::previous(Clip *p) { _previous = p; }
Clip *Clip::previous() { return _previous; }
void Clip::fetch()  { if (_playHead >= _start) { file.fetch(); }  }
void Clip::process() {}
std::size_t Clip::channels() const { return file.channels(); }
std::size_t Clip::offset() { return _offset; }
void Clip::offset(const std::size_t &argOffset) { _offset = argOffset; }
std::size_t Clip::start() { return _start; }
void Clip::start(const std::size_t &argStart) { _start = argStart; }
std::size_t Clip::end() { return _end; }
void Clip::end(const std::size_t &argEnd) { _end = argEnd; }
bool Clip::check() { return file.audioFile().error() == 0; }
