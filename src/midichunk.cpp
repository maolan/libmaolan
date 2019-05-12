#include <iostream>
#include <unistd.h>
#include <maolan/midichunk.h>
#include <maolan/midievent.h>


std::ostream & operator<<(std::ostream &os, const MIDIChunk &midi)
{
  if (midi.type == MIDIEvent::NOTE_ON)
  {
    os << "note = " << (unsigned int)midi.note << std::endl;
    os << "channel = " << (unsigned int)midi.channel << std::endl;
    os << "velocity = " << (unsigned int)midi.velocity << std::endl;
  }
  else if (midi.type == MIDIEvent::NOTE_OFF)
  {
    os << "note = " << (unsigned int)midi.note << std::endl;
    os << "channel = " << (unsigned int)midi.channel << std::endl;
  }
  else if (midi.type == MIDIEvent::CONTROLER_ON)
  {
    os << "controler = " << (unsigned int)midi.controler << std::endl;
    os << "channel = " << (unsigned int)midi.channel << std::endl;
    os << "value = " << (unsigned int)midi.value << std::endl;
  }
  else
  {
    return os;
  }
  os << std::endl;
  return os;
}
