#pragma once
#include <cstdint>
#include <maolan/audio/file.h>
#include <maolan/audio/io.h>

namespace maolan::audio
{
class Track;
class Clip : public IO
{
public:
  Clip(Track *parent, const std::size_t &channels);
  Clip(
    const std::string &path,
    const uint64_t &start,
    const uint64_t &end,
    const uint64_t &offset = 0,
    Track *parent = nullptr
  );
  ~Clip();

  void fetch();
  void process();
  std::size_t channels() const;
  uint64_t offset();
  void offset(const uint64_t &argOffset);
  uint64_t start();
  void start(const uint64_t &argStart);
  uint64_t end();
  void end(const uint64_t &argEnd);
  Buffer pull(const unsigned &channel);
  bool check();
  void write(const std::vector<Buffer> &frame);
  void next(Clip *n);
  Clip *next();
  void previous(Clip *n);
  Clip *previous();
  void parent(maolan::IO *p);

protected:
  uint64_t _offset;
  uint64_t _start;
  uint64_t _end;
  bool initialized = false;
  File file;
  Track *_parent;
  Clip *_next;
  Clip *_previous;
};
} // namespace maolan::audio
