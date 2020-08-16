#pragma once
#include <fstream>
#include <maolan/io.h>
#include <maolan/midi/buffer.h>
#include <string>


namespace maolan::midi
{
class File
{
public:
  File(const std::string &path);
  ~File();

  Buffer read();
  void readHeaders();
  bool eof();
  bool good();
  std::streampos tellg();

  float rate;
  unsigned division;
  unsigned headerLength;
  unsigned format;
  unsigned chunks;
  unsigned length;

protected:
  std::fstream file;
  Buffer last = nullptr;
};
} // namespace maolan::midi
