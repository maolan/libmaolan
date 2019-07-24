#include <maolan/audio/clip.h>
#include <maolan/audio/track.h>
#include <pugixml.hpp>

using namespace maolan::audio;

Track::Track() : IO(0, true), _current{nullptr}, first{nullptr}, last{nullptr}
{

  addClip(0,30000,0,"data/session.wav");
  _type = "Track";
}

void Track::fetch()
{
  auto clip = _current;
  if (clip != nullptr)
  {
    clip->fetch();
    clip->process();
  }
}

void Track::process() {}

void Track::addClip(const uint64_t &start, const uint64_t &end,
                    const uint64_t &offset, const std::string &path)
{
    Clip::create(start, end, offset, path, this);

}

std::size_t Track::channels() const
{
  if (_current == nullptr)
  {
    return 0;
  }
  return _current->channels();
}

Buffer Track::pull(const unsigned &channel)
{
  if (_current == nullptr)
  {
    return nullptr;
  }
  if (_playHead >= _current->start())
  {
    return _current->pull(channel);
  }
  return nullptr;
}

void Track::setup()
{
  if (first == nullptr)
  {
    _current = nullptr;
  }
  else if (_playHead < first->start())
  {
    _current = nullptr;
  }
  else if (_playHead > last->end())
  {
    _current = nullptr;
  }
  else
  {
    Clip *prevclip = nullptr;
    for (auto clip = last; clip != nullptr; clip = clip->previous())
    {
      if (clip->start() <= _playHead && clip->end() > _playHead)
      {
        _current = clip;
        break;
      }
      prevclip = clip;
    }
  }
}
