#pragma once
#include <cstdint>
#include <maolan/audio/file.h>
#include <maolan/audio/io.h>

namespace maolan
{
namespace audio
{
class Track;
class Clip : public IO
{
public:
  Clip(const uint64_t &start = 0, const uint64_t &end = 0,
       const uint64_t &offset = 0, const std::string &path = "",
       Track *parent = nullptr);
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
  static bool create(const uint64_t &start = 0, const uint64_t &end = 0,
       const uint64_t &offset = 0, const std::string &path = "",
       Track *parent = nullptr);
  bool check();

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
} // namespace audio
} // namespace maolan
