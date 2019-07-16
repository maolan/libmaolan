#pragma once
#include <iostream>
#include <maolan/io.h>
#include <maolan/midi/chunk.h>


class MIDIIO : public maolan::IO
{
public:
  MIDIChunk *get(int fd);
};
