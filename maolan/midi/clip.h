#pragma once
#include <maolan/midi/io.h>
#include <maolan/midi/buffer.h>
#include <maolan/midi/file.h>
#include <string>


namespace maolan::midi
{
class Clip : public IO
{
public:
  Clip(const std::string &name);
  void load();
  virtual void fetch();
  virtual void process();
  virtual void print();
  virtual Buffer pull();

protected:
  std::size_t _offset = 0;
  std::size_t _start = 0;
  std::size_t _end = 0;
  std::string _name;
  Buffer data;
  Buffer last;
  Buffer current;
  File file;
};
} // namespace maolan::midi
