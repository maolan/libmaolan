#include <iostream>
#include <unistd.h>

#include "maolan/midi/event.hpp"
#include "maolan/oss/midi/out.hpp"

using namespace maolan::midi;


static unsigned char buf[4];


OSSOut::OSSOut(const std::string &name, const std::string &device)
  : OSS{name, device}
{
  _type = "MIDIOSSOut";
  _name = device;
}


void OSSOut::fetch()
{
  OSS::fetch();
  _data = _inputs[0]->pull();
}


void OSSOut::process()
{
  for (auto buffer = _data; buffer != nullptr; buffer = buffer->next)
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
    write(_fd, buf, sizeof(buf));
  }
}
