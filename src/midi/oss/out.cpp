#include <iostream>
#include <unistd.h>
#include "maolan/midi/event.hpp"
#include "maolan/midi/oss/out.hpp"


using namespace maolan::midi;


static unsigned char buf[4];


OSSMIDIOut::OSSMIDIOut(const std::string &device) : OSSMIDI(device)
{
  _type = "MIDIOSSOut";
  _name = device;
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
