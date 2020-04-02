#include <iostream>
#include <maolan/config.h>
#include <maolan/audio/clip.h>
#include <maolan/audio/track.h>
#include <maolan/utils.h>
#include <pugixml.hpp>


using namespace maolan::audio;


Track::Track(const std::string &name, const std::size_t &ch)
    : IO(0, true)
    , Connectable(ch)
    , _current(nullptr)
    , first(nullptr)
    , last(nullptr)
    , armed(false)
    , muted(false)
    , soloed(false)
{
  _type = "Track";
  _name = name;
}


void Track::fetch()
{
  if (_current != nullptr)
  {
    _current->fetch();
    _current->process();
  }
}


void Track::process()
{
  if (armed)
  {
    std::vector<Buffer> frame;
    auto const chs = channels();
    frame.resize(chs);
    for (std::size_t channel = 0; channel < chs; ++channel)
    {
      frame[channel] = inputs[channel].pull();
    }
    recording->write(frame);
  }
}


Buffer Track::pull(const unsigned &channel)
{
  if (armed)
  {
    return nullptr;
  }
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
  if (armed && recording == nullptr) {
    recording = new Clip(this, channels());
    recording->start(_playHead);
    recording->end(_playHead + Config::audioBufferSize);
    _current = recording;
    first = _current;
    last = _current;
  }
  else if (first == nullptr)
  {
    _current = nullptr;
  }
  else if (_playHead < first->start())
  {
    _current = nullptr;
  }
  else if (!armed && _playHead > last->end())
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


void Track::add(Clip *clip)
{
  if (first == nullptr)
  {
    first = clip;
    last = clip;
  }
  else
  {
    for (auto cl = first; cl != nullptr; cl = cl->next())
    {
      if (clip->start() < cl->start())
      {
        clip->next(cl);
        clip->previous(cl->previous());
        cl->previous()->next(clip);
        cl->previous(clip);
        if (clip->previous() == nullptr) { first = clip; }
        if (clip->next() == nullptr) { last->next(clip); }
        break;
      }
    }

  }
}
std::size_t Track::channels() const { return inputs.size(); }
void Track::mute() { muted = !muted; }
void Track::arm() { armed = !armed; }
void Track::solo() { soloed = !soloed; }
