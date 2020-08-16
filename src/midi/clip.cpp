#include <exception>
#include <iostream>
#include <maolan/midi/buffer.h>
#include <maolan/midi/clip.h>
#include <maolan/midi/event.h>
#include <maolan/midi/file.h>


using namespace maolan::midi;


Clip::Clip(const std::string &name) : _name{name}, file{name} { load(); }


void Clip::fetch() {}


void Clip::process() {}


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


void Clip::print()
{
  for (auto buffer = data; buffer != nullptr; buffer = buffer->next)
  {
    buffer->print();
  }
}


Buffer Clip::pull()
{
  if (data == nullptr || current == nullptr)
  {
    return nullptr;
  }
  auto tempo = Config::tempos[Config::tempoIndex];
  if (current->time * tempo.ratio > _playHead)
  {
    return nullptr;
  }
  Buffer result = std::make_shared<BufferData>();
  Buffer lastBuffer;
  *result = *current;
  result->next = nullptr;
  current = current->next;
  result->channel = 2;
  return result;
}
