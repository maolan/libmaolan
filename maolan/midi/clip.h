#pragma once
#include <maolan/io.h>
#include <maolan/midi/chunk.h>
#include <string>
#include <vector>


class MIDIClip : public maolan::IO
{
public:
  MIDIClip();
  ~MIDIClip();

  MIDIChunk *next();
  void load(const std::string &filename);
  void fetch();
  void process();

protected:
  std::vector<MIDIChunk *> data;
};
