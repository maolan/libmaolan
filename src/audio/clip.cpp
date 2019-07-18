#include <maolan/audio/clip.h>
#include <maolan/audio/track.h>
#include <sndfile.hh>
#include <iostream>

using namespace maolan::audio;


Clip::Clip(const uint64_t &start, const uint64_t &end, const uint64_t &offset,
           const std::string &path, Track *parrent)
    : IO(0, true, false), _offset{offset}, _start{start}, _end{end},
      _previous{nullptr}, _next{nullptr}, file(path, offset)
{
  _type = "Clip";
  _parrent = parrent;
  if (_parrent != nullptr)
  {
    if (_parrent->last != nullptr)
    {
      _parrent->last->_next = this;
    }
    this->_previous = _parrent->last;
    _parrent->last = this;
    if (_parrent->first == nullptr)
    {
      _parrent->first = this;
    }
  }
}

Clip::~Clip()
{
  if (_parrent != nullptr)
  {
    if (this->_previous != nullptr)
    {
      this->_previous->next(this->_next);
    }
    else
    {
      _parrent->first = this->_next;
    }
    if (this->_next != nullptr)
    {
      this->_next->previous(this->_previous);
    }
    else
    {
      _parrent->last = this->_previous;
    }
  }
}

void Clip::next(Clip *n) { _next = n; }


Clip *Clip::next() { return _next; }


void Clip::previous(Clip *p) { _previous = p; }


Clip *Clip::previous() { return _previous; }

void Clip::fetch()
{
  if (_playHead >= _start)
  {
    file.fetch();
    file.process();
  }
}

void Clip::process() {}

std::size_t Clip::channels() const { return file.channels(); }

uint64_t Clip::offset() { return _offset; }

void Clip::offset(const uint64_t &argOffset) { _offset = argOffset; }

uint64_t Clip::start() { return _start; }

void Clip::start(const uint64_t &argStart) { _start = argStart; }

uint64_t Clip::end() { return _end; }

void Clip::end(const uint64_t &argEnd) { _start = argEnd; }

Buffer Clip::pull(const unsigned &channel)
{
  if (_playHead >= _start)
  {
    return file.pull(channel);
  }
  return nullptr;
}
