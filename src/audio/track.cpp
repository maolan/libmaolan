#include <algorithm>
#include <iostream>
#include <random>
#include <string>

#include "maolan/audio/clip.hpp"
#include "maolan/audio/track.hpp"
#include "maolan/config.hpp"


using namespace maolan::audio;


std::vector<Track *> Track::all;


std::string random_string(const std::size_t &size)
{
  std::string str("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
  std::random_device rd;
  std::mt19937 generator(rd());
  std::shuffle(str.begin(), str.end(), generator);
  return str.substr(0, size);
}


Track::Track(const std::string &name, const std::size_t &ch)
    : IO(0, true, true), Connectable(ch), muted(false), armed(false),
      soloed(false), first(nullptr), _current(nullptr), last(nullptr)
{
  _type = "AudioTrack";
  _name = name;
  outputs.resize(ch);
  all.push_back(this);
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
  auto frame = new Frame(chs, 0, 0);
  if (armed)
  {
    for (std::size_t channel = 0; channel < chs; ++channel)
    {
      frame->audio[channel] = _inputs[channel].pull();
    }
    recording->write(frame);
    if (!muted)
    {
      outputs = frame->audio;
    }
    delete frame;
  }
  else if (!muted && _current != nullptr && _playHead >= _current->start())
  {
    for (std::size_t channel = 0; channel < chs; ++channel)
    {
      frame->audio[channel] = _current->pull(channel);
    }
    auto &result = frame;
    // TODO: process plugins

    outputs = result->audio;
    delete result;
  }
}


void Track::setup()
{
  if (armed && recording == nullptr)
  {
    recording = new Clip(random_string(8), this);
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
  clip->next(nullptr);
  clip->previous(nullptr);
  if (first == nullptr)
  {
    first = clip;
    last = clip;
  }
  else if (first->start() > clip->start())
  {
    first->previous(clip);
    clip->next(first);
    first = clip;
  }
  else if (last->start() < clip->start())
  {
    last->next(clip);
    clip->previous(last);
    last = clip;
  }
  else
  {
    for (auto cl = last; cl != nullptr; cl = cl->previous())
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


void Track::remove(plugin::lv2::Plugin *plugin)
{
  for (std::size_t i = 0; i < _plugins.size(); ++i)
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


nlohmann::json Track::json()
{
  auto data = IO::json();
  data["clips"] = nlohmann::json::array();
  for (auto clip = first; clip != nullptr; clip = clip->next())
  {
    data["clips"].push_back(clip->json());
  }
  return data;
}


nlohmann::json Track::connections() { return conns(_name); }
void Track::add(plugin::lv2::Plugin *plugin) { _plugins.push_back(plugin); }
std::size_t Track::channels() const { return _inputs.size(); }
bool Track::mute() { return muted; }
bool Track::arm() { return armed; }
bool Track::solo() { return soloed; }
void Track::mute(const bool &value) { muted = value; }
void Track::arm(const bool &value) { armed = value; }
void Track::solo(const bool &value) { soloed = value; }
Clip *Track::clips() { return first; }
Track::~Track() { std::remove(all.begin(), all.end(), this); }
