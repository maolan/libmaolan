#pragma once
#include <fstream>
#include <string>
#include "maolan/io.hpp"
#include "maolan/midi/buffer.hpp"


namespace maolan::midi
{
class File : public IO
{
public:
  File(const std::string &path);
  ~File();

  virtual void fetch();
  virtual void process();
  Buffer read();
  void save(const Buffer buffer);
  void readHeaders();
  bool eof();
  bool good();
  std::streampos tellg();

protected:
  std::fstream file;
  midi::Buffer last;
  float rate;
  std::uint16_t chunks;
  std::uint16_t division;
  std::uint16_t format;
  std::uint32_t headerLength;
  std::uint32_t length;
};
} // namespace maolan::midi
