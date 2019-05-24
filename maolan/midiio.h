#pragma once
#include <iostream>
#include <maolan/io.h>
#include <maolan/midichunk.h>


class MIDIIO : public IO
{
  public:
    MIDIChunk * get(int fd);
};
