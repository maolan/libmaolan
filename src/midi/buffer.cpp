#include <iostream>
#include <maolan/midi/buffer.h>
#include <maolan/midi/event.h>


using namespace maolan::midi;


BufferData::~BufferData()
{
  if (data != nullptr)
  {
    delete []data;
  }
}


void
BufferData::print()
{
  switch (type)
  {
    case Event::NOTE_ON:
    case Event::NOTE_OFF:
      std::cout << "note = " << (unsigned)note << '\n';
      std::cout << "channel = " << (unsigned)channel << '\n';
      std::cout << "velocity = " << (unsigned)velocity << '\n';
      std::cout << "time = " << time << '\n';
      break;
    case Event::CONTROLER_ON:
      std::cout << "controller = " << (unsigned)controller << '\n';
      std::cout << "channel = " << (unsigned)channel << '\n';
      std::cout << "value = " << (unsigned)value << '\n';
      break;
    case Event::META:
      std::cout << "meta: " << std::hex << meta << '\n';
      break;
  }
  std::cout << std::endl;
}
