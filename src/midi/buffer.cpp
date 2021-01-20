#include <iostream>
#include "maolan/midi/buffer.hpp"
#include "maolan/midi/event.hpp"


using namespace maolan::midi;


BufferData::~BufferData()
{
  if (data != nullptr)
  {
    delete[] data;
  }
}


void BufferData::print()
{
  switch (type)
  {
    case Event::NOTE_ON:
    case Event::NOTE_OFF:
    {
      auto typeStr = type == Event::NOTE_ON ? "note on" : "note off";
      std::cout << "type = " << typeStr << '\n';
      std::cout << "note = " << (unsigned)note << '\n';
      std::cout << "channel = " << (unsigned)channel << '\n';
      std::cout << "velocity = " << (unsigned)velocity << '\n';
      break;
    }
    case Event::CONTROLER_ON:
    {
      std::cout << "controller = " << (unsigned)controller << '\n';
      std::cout << "channel = " << (unsigned)channel << '\n';
      std::cout << "value = " << (unsigned)value << '\n';
      break;
    }
    case Event::META:
    {
      std::cout << "meta: " << std::hex << (unsigned)meta << std::dec << '\n';
      break;
    }
    default:
    {
      std::cout << "Unknown event of type " << (unsigned)type << '\n';
    }
  }
  std::cout << "time = " << time << '\n';
  std::cout << std::endl;
}
