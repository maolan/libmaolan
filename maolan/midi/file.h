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
  void save(const Buffer buffer);
  void readHeaders();
  bool eof();
  bool good();
  std::streampos tellg();

protected:
  std::fstream file;
  std::string _path;
  midi::Buffer last;
  float rate;
  std::uint16_t chunks;
  std::uint16_t division;
  std::uint16_t format;
  std::uint32_t headerLength;
  std::uint32_t length;
};
} // namespace maolan::midi
