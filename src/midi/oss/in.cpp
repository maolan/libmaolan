#include <maolan/constants.h>
#include <maolan/midi/event.h>
#include <maolan/midi/oss/in.h>
#include <unistd.h>


using namespace maolan::midi;


static BufferData *lastBuffer = nullptr;


OSSMIDIIn::OSSMIDIIn(const std::string &device)
  : OSSMIDI(device)
{
  _type = "OSSMIDIIn";
  _name = "OSS MIDI In";
}


void
OSSMIDIIn::setup()
{
  data = std::make_shared<BufferData>();
  lastBuffer = nullptr;
}


void
OSSMIDIIn::fetch()
{
  static int l = -1;
  static unsigned char buf[8];
  BufferData *chunk;

  while ((l = read(device->fd, buf, sizeof(buf))) != -1)
  {
    if (lastBuffer == nullptr)
    {
      chunk = data.get();
    } else {
      chunk = new BufferData;
      lastBuffer->next = chunk;
    }
    chunk->type = buf[0] & MIDIEvent::NOTE_MASK;
    if (chunk->type == MIDIEvent::NOTE_ON)
    {
      chunk->channel = buf[0] & MIDIEvent::CHANNEL_MASK;
      chunk->note = buf[1];
      chunk->velocity = buf[2];
    }
    else if (chunk->type == MIDIEvent::NOTE_OFF)
    {
      chunk->channel = buf[0] & MIDIEvent::CHANNEL_MASK;
      chunk->note = buf[1];
      chunk->velocity = buf[2];
    }
    else if (chunk->type == MIDIEvent::CONTROLER_ON)
    {
      chunk->channel = buf[0] & MIDIEvent::CHANNEL_MASK;
      chunk->controler = buf[1];
      chunk->value = buf[2];
    }
    lastBuffer = chunk;
  }
}


void
OSSMIDIIn::process()
{}


Buffer
OSSMIDIIn::pull()
{
  return data;
}
