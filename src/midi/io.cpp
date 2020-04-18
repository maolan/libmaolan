#include <iostream>
#include <cstring>
#include <maolan/midi/chunk.h>
#include <maolan/midi/event.h>
#include <maolan/midi/io.h>
#include <unistd.h>


MIDIChunk *MIDIIO::get(int fd)
{
  int l;
  unsigned char buf[8];
  MIDIChunk *chunk = new MIDIChunk;
  if ((l = read(fd, buf, sizeof(buf))) != -1)
  {
    chunk->type = buf[0] & MIDIEvent::NOTE_MASK;
    if (chunk->type == MIDIEvent::NOTE_ON)
    {
      chunk->channel = buf[0] & MIDIEvent::CHANNEL_MASK;
      chunk->note = buf[1];
      chunk->velocity = buf[2];
      std::cout << chunk;
    }
    else if (chunk->type == MIDIEvent::NOTE_OFF)
    {
      chunk->channel = buf[0] & MIDIEvent::CHANNEL_MASK;
      chunk->note = buf[1];
      std::cout << chunk;
    }
    else if (chunk->type == MIDIEvent::CONTROLER_ON)
    {
      chunk->channel = buf[0] & MIDIEvent::CHANNEL_MASK;
      chunk->controler = buf[1];
      chunk->value = buf[2];
      std::cout << chunk;
    }
  }
  else if (errno != EAGAIN)
  {
    std::cerr << "Error code: " << errno << std::endl;
    std::cerr << std::strerror(errno) << std::endl;
  }
  return chunk;
}
