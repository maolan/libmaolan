#include <maolan/audio/clip.h>
#include <maolan/audio/track.h>
#include <maolan/config.h>
#include <maolan/utils.h>


using namespace maolan::audio;


Track::Track(const std::string &name, const std::size_t &ch)
    : IO(0, true, true), Connectable(ch), _current(nullptr), first(nullptr),
      last(nullptr), armed(false), muted(false), soloed(false)
{
  _type = "Track";
  _name = name;
  outputs.resize(ch);
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
  if (_current != nullptr)
  {
    _current->process();
  }
  auto const chs = channels();
  auto frame = new Frame(chs, 0);
  if (armed)
  {
    for (std::size_t channel = 0; channel < chs; ++channel)
    {
      frame->audioBuffer[channel] = inputs[channel].pull();
    }
    recording->write(frame);
    if (!muted)
    {
      outputs = frame->audioBuffer;
    }
    delete frame;
  }
  else if (!muted && _current != nullptr && _playHead >= _current->start())
  {
    for (std::size_t channel = 0; channel < chs; ++channel)
    {
      frame->audioBuffer[channel] = _current->pull(channel);
    }
    auto &result = frame;
    if (_plugins.size() > 0)
    {
      bool inInput = true;
      auto frame2 = new Frame(chs, 0);
      for (auto &plugin : _plugins)
      {
        if (inInput)
        {
          frame2 = (Frame *)plugin->process(result);
          result = frame2;
        }
        else
        {
          frame = (Frame *)plugin->process(result);
          result = frame;
        }
        inInput = !inInput;
      }
    }

    outputs = result->audioBuffer;
    delete result;
  }
}


void Track::setup()
{
  if (armed && recording == nullptr)
  {
    recording = new Clip(this, channels());
    recording->start(_playHead);
    recording->end(_playHead + Config::audioBufferSize);
    _current = recording;
    first = _current;
    last = _current;
    _current->setup();
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
    for (auto clip = last; clip != nullptr; clip = clip->previous())
    {
      if (clip->start() <= _playHead && clip->end() > _playHead)
      {
        _current = clip;
        _current->setup();
        break;
      }
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
        if (clip->previous() == nullptr)
        {
          first = clip;
        }

        // This will never happen, so it's a bug
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


void Track::remove(Plugin *plugin)
{
  for (int i = 0; i < _plugins.size(); ++i)
  {
    if (_plugins[i] == plugin)
    {
      _plugins.erase(_plugins.begin() + i);
      return;
    }
  }
}


Buffer Track::pull(const std::size_t &channel)
{
  if (muted)
  {
    return nullptr;
  }
  return IO::pull(channel);
}


void Track::add(Plugin *plugin) { _plugins.push_back(plugin); }
std::size_t Track::channels() const { return inputs.size(); }
void Track::mute() { muted = !muted; }
void Track::arm() { armed = !armed; }
void Track::solo() { soloed = !soloed; }
