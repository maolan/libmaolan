#pragma once
#include <fstream>
#include <maolan/io.h>
#include <maolan/midi/chunk.h>
#include <string>


class MIDIFile
{
public:
  MIDIFile(const std::string &path);
  ~MIDIFile();

  MIDIChunk *read();
  void skipHeaders();
  bool eof();

  int division;

protected:
  std::ifstream file;
};
