#include <iostream>
#include <maolan/midi/event.h>
#include <maolan/midi/oss/out.h>
#include <unistd.h>


using namespace maolan::midi;


static unsigned char buf[4];


OSSMIDIOut::OSSMIDIOut(const std::string &device) : OSSMIDI(device)
{
  _type = "OSSMIDIOut";
  _name = "OSS MIDI Out";
}


void OSSMIDIOut::fetch()
{
  Connectable::fetch();
  data = inputs[0].pull();
}


void OSSMIDIOut::process()
{
  for (auto buffer = data; buffer != nullptr; buffer = buffer->next)
  {
    if (buffer->type == 0)
    {
      continue;
    }
    buf[0] = buffer->type | buffer->channel;
    if (buffer->type == Event::CONTROLER_ON)
    {
      buf[1] = buffer->controller;
      buf[2] = buffer->value;
    }
    else
    {
      buf[1] = buffer->note;
      buf[2] = buffer->velocity;
    }
    buf[3] = '\0';
    write(device->fd, buf, sizeof(buf));
  }
}
