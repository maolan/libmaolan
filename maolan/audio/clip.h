#pragma once
#include <cstdint>
#include <maolan/audio/file.h>
#include <maolan/audio/io.h>
#include <sndfile.hh>

namespace maolan
{
namespace audio
{
class Clip : public IO
{
public:
  Clip(const uint64_t &start = 0, const uint64_t &offset = 0,
       const std::string &path = "");

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
  uint64_t playHeadCheck();

protected:
  uint64_t _offset;
  uint64_t _start;
  uint64_t _end;
  bool initialized = false;
  File file;
};
} // namespace audio
} // namespace maolan
