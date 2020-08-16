#include <exception>
#include <iostream>
#include <maolan/midi/buffer.h>
#include <maolan/midi/clip.h>
#include <maolan/midi/event.h>
#include <maolan/midi/file.h>


using namespace maolan::midi;


// TODO: Once track is implemented, only IO(name) is needed
Clip::Clip(const std::string &name)
    : IO(name, true, true), _name{name}, file{name}
{
  load();
}


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
  while (current != nullptr && (current->time - tempo.tick) * tempo.ratio < nextTime)
  {
    Buffer buffer = std::make_shared<BufferData>();
    *buffer = *current;
    buffer->next = nullptr;
    // TODO: Once track is implemented, use track's channel settings
    buffer->channel = 2;
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
    _end = last->time;
  }
  catch (std::exception &e)
  {
    std::cerr << "Error loading file " << _name << ": ";
    std::cerr << e.what() << '\n';
    std::cerr << "Read bytes so far: " << file.tellg() << '\n';
  }
}


Buffer Clip::pull() { return output; }
