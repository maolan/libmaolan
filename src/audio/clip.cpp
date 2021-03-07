#include <iostream>
#include "maolan/audio/clip.hpp"
#include "maolan/audio/track.hpp"
#include "maolan/config.hpp"


using namespace maolan::audio;


Clip::Clip(Track *parent, const std::size_t &ch)
  : IO(0, true, false)
  , file(ch)
{
  _type = "Clip";
  if (parent != nullptr) { parent->add(this); }
}


Clip::Clip(const std::string &path, const std::size_t &start,
           const std::size_t &end, const std::size_t &offset, Track *parent)
    : IO{0, true, false, path}, _offset{offset}, _start{start}, _end{end},
      _previous{nullptr}, _next{nullptr}, file{path, offset}, _parent{parent}
{
  _type = "Clip";
  if (parent) { parent->add(this); }
}


Clip::~Clip()
{
  if (_parent) { _parent->remove(this); }
}


Buffer Clip::pull(const unsigned &channel)
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


void Clip::write(const Frame *const fr) { write(*fr); }
void Clip::next(Clip *n) { _next = n; }
Clip *Clip::next() { return _next; }
void Clip::previous(Clip *p) { _previous = p; }
Clip *Clip::previous() { return _previous; }
void Clip::fetch() { if (_playHead >= _start) { file.fetch(); } }
void Clip::process() {}
std::size_t Clip::channels() const { return file.channels(); }
std::size_t Clip::offset() { return _offset; }
void Clip::offset(const std::size_t &argOffset) { _offset = argOffset; }
std::size_t Clip::start() { return _start; }
void Clip::start(const std::size_t &argStart) { _start = argStart; }
std::size_t Clip::end() { return _end; }
void Clip::end(const std::size_t &argEnd) { _end = argEnd; }
bool Clip::check() { return file.audioFile().error() == 0; }
