#include <exception>
#include <iostream>
#include <maolan/midi/buffer.h>
#include <maolan/midi/clip.h>
#include <maolan/midi/event.h>
#include <maolan/midi/file.h>
#include <maolan/midi/track.h>


using namespace maolan::midi;


std::vector<Clip *> Clip::clips;


void Clip::saveAll()
{
  for (const auto &clip : clips)
  {
    clip->save();
  }
}


Clip::Clip(const std::string &name, Track *parent)
    : IO(name), file(name), _previous{nullptr}, _next{nullptr}, _parent{parent},
      _start{0}, _end{100000000}, _offset{0}
{
  _type = "MIDIClip";
  load();
  if (parent != nullptr)
  {
    parent->add(this);
  }
  clips.push_back(this);
}


Clip::Clip(const std::string &name, const std::size_t &start,
           const std::size_t &end, const std::size_t &offset, Track *parent)
    : IO(name), _name{name}, file{name}, _previous{nullptr}, _next{nullptr},
      _parent{parent}, _start{start}, _end{end}, _offset{offset}
{
  _type = "MIDIClip";
  load();
  if (parent != nullptr)
  {
    parent->add(this);
  }
}


Clip::~Clip()
{
  if (_parent != nullptr)
  {
    _parent->remove(this);
  }
  auto newend = std::remove_if(clips.begin(), clips.end(),
                               [this](const Clip *c) { return c == this; });
  clips.erase(newend, clips.end());
}


void Clip::setup() {}


void Clip::fetch() {}


void Clip::process()
{
  output = nullptr;
  if (data == nullptr || current == nullptr)
  {
    return;
  }
  auto tempo = Config::tempos[Config::tempoIndex];
  auto playhead = _playHead - tempo.time;
  if (playhead < (current->time - tempo.tick) * tempo.ratio)
  {
    return;
  }
  Buffer last;
  auto nextTime = playhead + Config::audioBufferSize;
  while (current != nullptr &&
         (current->time - tempo.tick) * tempo.ratio < nextTime)
  {
    Buffer buffer = std::make_shared<BufferData>();
    *buffer = *current;
    buffer->next = nullptr;
    if (output == nullptr)
    {
      output = buffer;
      last = output;
    }
    else
    {
      last->next = buffer;
      last = buffer;
    }
    current = current->next;
  }
}


void Clip::load()
{
  data = nullptr;
  last = nullptr;
  float divisionRate;
  try
  {
    file.readHeaders();
    while (true)
    {
      Buffer chunk = file.read();
      if (chunk->type == Event::META)
      {
        if (chunk->meta == 0x2f)
        {
          break;
        }
      }
      else
      {
        if (data == nullptr)
        {
          data = chunk;
          current = data;
        }
        else
        {
          last->next = chunk;
        }
        last = chunk;
      }
    }
  }
  catch (std::exception &e)
  {
    std::cerr << "Error loading file " << _name << ": ";
    std::cerr << e.what() << '\n';
    std::cerr << "Read bytes so far: " << file.tellg() << '\n';
  }
}


void Clip::parent(maolan::IO *p)
{
  if (_parent != nullptr)
  {
    _parent->remove(this);
  }
  _parent = (Track *)p;
  if (_parent != nullptr)
  {
    _parent->add(this);
  }
}


const std::size_t Clip::startSample() const
{
  auto tempo = Config::tempos[Config::tempoIndex];
  return (_start - tempo.tick) * tempo.ratio + tempo.time;
}


const std::size_t Clip::endSample() const
{
  auto tempo = Config::tempos[Config::tempoIndex];
  return (_end - tempo.tick) * tempo.ratio + tempo.time;
}


void Clip::write(const Buffer buffer)
{
  if (buffer == nullptr)
  {
    return;
  }
  if (data == nullptr)
  {
    data = buffer;
    last = data;
    current = data;
    return;
  }
  last->next = buffer;
  for (last = buffer; last->next != nullptr; last = last->next)
  {
  }
}


void Clip::save()
{
  file.save(data);
}


const std::size_t &Clip::start() const { return _start; }
const std::size_t &Clip::end() const { return _end; }
void Clip::next(Clip *n) { _next = n; }
Clip *Clip::next() { return _next; }
void Clip::previous(Clip *p) { _previous = p; }
Clip *Clip::previous() { return _previous; }
