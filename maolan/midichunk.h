#pragma once
#include <ostream>


class MIDIChunk
{
  public:
    int controler;
    int channel;
    int value;
    unsigned int time;
    unsigned char note;
    unsigned char type;
    unsigned char velocity;
    char *data;

    friend std::ostream& operator<<(std::ostream& os, const MIDIChunk &midi);
};
