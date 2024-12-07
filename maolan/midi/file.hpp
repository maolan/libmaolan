#pragma once
#include <fstream>
#include <string>

#include <maolan/midi/buffer.hpp>

namespace maolan::midi {
class File {
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
  std::fstream _file;
  midi::Buffer _last;
  float _rate;
  std::uint16_t _chunks;
  std::uint16_t _division;
  std::uint16_t _format;
  std::uint32_t _headerLength;
  std::uint32_t _length;
  std::string _name;
  std::string _type;
};
} // namespace maolan::midi
