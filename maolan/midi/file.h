#pragma once
#include <fstream>
#include <maolan/io.h>
#include <maolan/midi/buffer.h>
#include <string>


namespace maolan::midi
{
class MIDIFile
{
public:
  MIDIFile(const std::string &path);
  ~MIDIFile();

  BufferData * read();
  void skipHeaders();
  bool eof();

  int division;

protected:
  std::ifstream file;
};
} // namespace maolan::midi
