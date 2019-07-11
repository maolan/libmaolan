#pragma once
#include <iostream>


class MIDIEvent
{
  public:
    static unsigned char const NOTE_MASK = 0xF0;
    static unsigned char const NOTE_ON = 0x90;
    static unsigned char const NOTE_OFF = 0x80;
    static unsigned char const CHANNEL_MASK = 0xF;
    static unsigned char const CONTROLER_ON = 0xB0;
    static unsigned char const META = 0xFF;
};
