#pragma once
#include <cstdint>
#include <maolan/audio/file.h>
#include <maolan/audio/io.h>
#include <maolan/frame.h>


namespace maolan::audio
{
class Track;
class Clip : public IO
{
public:
  Clip(Track *parent, const std::size_t &channels);
  Clip(const std::string &path, const uint64_t &start, const uint64_t &end,
       const uint64_t &offset = 0, Track *parent = nullptr);
  ~Clip();

  virtual void fetch();
  virtual void process();
  virtual void setup();
  virtual Buffer pull(const unsigned &channel);

  std::size_t channels() const;
  uint64_t offset();
  void offset(const uint64_t &argOffset);
  uint64_t start();
  void start(const uint64_t &argStart);
  uint64_t end();
  void end(const uint64_t &argEnd);
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
  bool initialized = false;
  File file;
  Track *_parent;
  Clip *_next;
  Clip *_previous;
};
} // namespace maolan::audio
