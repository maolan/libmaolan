#include <iostream>
#include <unistd.h>

#include "maolan/oss/midi/out.hpp"
#include "maolan/midi/event.hpp"

using namespace maolan::midi;


static unsigned char buf[4];


OSSMIDIOut::OSSMIDIOut(const std::string &device) : OSSMIDI(device)
{
  _type = "MIDIOSSOut";
  _name = device;
}


void OSSMIDIOut::fetch()
{
  OSSMIDI::fetch();
  data = _inputs[0]->pull();
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
