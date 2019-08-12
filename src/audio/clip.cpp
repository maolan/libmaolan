#include <maolan/audio/clip.h>
#include <maolan/audio/track.h>
#include <sndfile.hh>


using namespace maolan::audio;


Clip::Clip(const uint64_t &start, const uint64_t &end, const uint64_t &offset,
           const std::string &path, Track *parent)
    : IO(0, true, false), _offset{offset}, _start{start}, _end{end},
      _previous{nullptr}, _next{nullptr}, file(path, offset)
{
  _type = "Clip";
}

Clip::~Clip()
{
  if (_parent != nullptr)
  {
    if (this->_previous != nullptr)
    {
      this->_previous->next(this->_next);
    }
    else
    {
      _parent->first = this->_next;
    }
    if (this->_next != nullptr)
    {
      this->_next->previous(this->_previous);
    }
    else
    {
      _parent->last = this->_previous;
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

bool Clip::create(const uint64_t &start, const uint64_t &end,
                  const uint64_t &offset, const std::string &path,
                  Track *parent)
{
  if (start > end or offset > (end - start))
  {
    return false;
  }
  Clip *clip = new Clip(start, end, offset, path);
  if (clip->check())
  {
    clip->parent(parent);
    return true;
  }
  return false;
}

bool Clip::check()
{
  if (file.audioFile().error() != 0)
  {
    return false;
  }
  return true;
}

void Clip::parent(maolan::IO *p)
{
  _parent = (Track *)p;
  if (_parent != nullptr)
  {
    if (_parent->last != nullptr)
    {
      _parent->last->_next = this;
    }
    _previous = _parent->last;
    _parent->last = this;
    if (_parent->first == nullptr)
    {
      _parent->first = this;
    }
  }
}
