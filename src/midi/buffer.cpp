#include <iostream>
#include <maolan/midi/buffer.h>
#include <maolan/midi/event.h>


using namespace maolan::midi;


BufferData::BufferData()
  : type{0}
  , data{nullptr}
  , next{nullptr}
{}


void
BufferData::print()
{
  if (type == MIDIEvent::NOTE_ON)
  {
    std::cout << "note = " << note << '\n';
    std::cout << "channel = " << channel << '\n';
    std::cout << "velocity = " << velocity << '\n';
  }
  else if (type == MIDIEvent::NOTE_OFF)
  {
    std::cout << "note = " << note << '\n';
    std::cout << "channel = " << channel << '\n';
    std::cout << "velocity = " << velocity << '\n';
  }
  else if (type == MIDIEvent::CONTROLER_ON)
  {
    std::cout << "controler = " << controler << '\n';
    std::cout << "channel = " << channel << '\n';
    std::cout << "value = " << value << '\n';
  }
  std::cout << std::endl;
}
