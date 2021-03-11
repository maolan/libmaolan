#include <iomanip>
#include <unistd.h>
#include "maolan/constants.hpp"
#include "maolan/midi/event.hpp"
#include "maolan/midi/oss/in.hpp"


using namespace maolan::midi;


static Buffer lastBuffer = nullptr;


OSSMIDIIn::OSSMIDIIn(const std::string &device) : OSSMIDI(device)
{
  _type = "MIDIOSSIn";
  _name = "OSS MIDI In";
}


void OSSMIDIIn::setup()
{
  data = std::make_shared<BufferData>();
  lastBuffer = nullptr;
}


void OSSMIDIIn::fetch()
{
  static int l = -1;
  static unsigned char buf[8];
  Buffer chunk;

  while ((l = read(device->fd, buf, sizeof(buf))) != -1)
  {
    if (lastBuffer == nullptr)
    {
      chunk = data;
    }
    else
    {
      chunk = std::make_shared<BufferData>();
      lastBuffer->next = chunk;
    }
    chunk->type = buf[0] & Event::NOTE_MASK;
    chunk->channel = buf[0] & Event::CHANNEL_MASK;
    if (chunk->type == Event::NOTE_ON || chunk->type == Event::NOTE_OFF)
    {
      chunk->note = buf[1];
      chunk->velocity = buf[2];
    }
    else if (chunk->type == Event::CONTROLER_ON)
    {
      chunk->controller = buf[1];
      chunk->value = buf[2];
    }
    lastBuffer = chunk;
  }
}


void OSSMIDIIn::process() {}


Buffer OSSMIDIIn::pull() { return data; }
