#pragma once
#include "maolan/audio/file.hpp"
#include "maolan/audio/io.hpp"
#include "maolan/frame.hpp"


namespace maolan::audio
{
class Track;
class Clip : public audio::IO
{
public:
  Clip(const std::string &path, Track *parent = nullptr,
       const std::size_t &start = 0, const std::size_t &end = 0,
       const std::size_t &offset = 0);
  ~Clip();

  virtual void fetch();
  virtual void process();
  virtual Buffer pull(const std::size_t &channel = 0);

  std::size_t channels() const;
  std::size_t offset();
  void offset(const std::size_t &argOffset);
  std::size_t start();
  void start(const std::size_t &argStart);
  std::size_t end();
  void end(const std::size_t &argEnd);
  bool check();

  void write(const Frame &frame);
  void write(const Frame *const frame);

  void next(Clip *n);
  Clip *next();
  void previous(Clip *n);
  Clip *previous();
  void parent(maolan::IO *p);

protected:
  std::size_t _offset;
  std::size_t _start;
  std::size_t _end;
  File file;
  Track *_parent;
  Clip *_next;
  Clip *_previous;
};
} // namespace maolan::audio
