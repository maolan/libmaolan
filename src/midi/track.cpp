#include <maolan/midi/clip.h>
#include <maolan/midi/track.h>


using namespace maolan::midi;


Track::Track(const std::string &name, const std::size_t &channel)
    : IO(name, true, true), Connectable(), _current(nullptr), first(nullptr),
      last(nullptr), armed(false), muted(false),
      soloed(false), _channel{channel}
{
  _type = "MIDITrack";
  _name = name;
}


void Track::fetch()
{
  Connectable::fetch();
  if (_current != nullptr)
  {
    _current->fetch();
  }
}


void Track::process()
{
  Connectable::process();
  if (_current == nullptr)
  {
    output = nullptr;
    return;
  }
  if (armed)
  {
  }
  else if (!muted && _playHead >= _current->startSample())
  {
    _current->process();
    output = _current->pull();
    for (auto buffer = output; buffer != nullptr; buffer = buffer->next)
    {
      buffer->channel = _channel;
    }
  }
}


void Track::setup()
{
  if (armed && recording == nullptr)
  {
    recording = new Clip("/tmp/clip.mid", this);
    recording->startSample(_playHead);
    recording->endSample(_playHead + Config::audioBufferSize);
    _current = recording;
    first = _current;
    last = _current;
    _current->setup();
  }
  else if (first == nullptr)
  {
    _current = nullptr;
  }
  else if (_playHead < first->startSample())
  {
    _current = nullptr;
  }
  else if (!armed && _playHead > last->endSample())
  {
    _current = nullptr;
  }
  else
  {
    for (auto clip = last; clip != nullptr; clip = clip->previous())
    {
      if (clip->startSample() <= _playHead && _playHead < clip->endSample())
      {
        _current = clip;
        break;
      }
    }
    _current->setup();
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
        if (clip->previous() == nullptr)
        {
          first = clip;
        }
        if (clip->next() == nullptr)
        {
          last->next(clip);
        }
        break;
      }
    }
  }
}


void Track::remove(Clip *clip)
{
  if (first == nullptr)
  {
    return;
  }
  for (auto cl = first; cl != nullptr; cl = cl->next())
  {
    if (cl == clip)
    {
      if (cl->next() != nullptr)
      {
        cl->next()->previous(cl->previous());
      }
      if (cl->previous() != nullptr)
      {
        cl->previous()->next(cl->next());
      }
      if (cl == _current)
      {
        _current = nullptr;
      }
      if (cl == recording)
      {
        recording = nullptr;
      }
      if (cl == first)
      {
        first = cl->next();
      }
      if (cl == last)
      {
        last = cl->previous();
      }
      cl->parent(nullptr);
      return;
    }
  }
}


Buffer Track::pull()
{
  if (muted)
  {
    return nullptr;
  }
  return output;
}


void Track::mute() { muted = !muted; }
void Track::arm() { armed = !armed; }
void Track::solo() { soloed = !soloed; }

