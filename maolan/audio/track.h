/* vim: set syntax=cpp: */
#pragma once
#include <maolan/audio/io.h>

namespace maolan
{
namespace audio
{
class Clip;
class Track : public IO
{
public:
  Track();

  void fetch();
  void process();
  void addClip(const uint64_t &start, const uint64_t &end,
               const uint64_t &offset, const std::string &path);
  std::size_t channels() const;
  Buffer pull(const unsigned &channel);

  void setup();

  friend class Clip;

protected:
  bool initialized = false;
  Clip *first;
  Clip *_current;
  Clip *last;
};
} // namespace audio
} // namespace maolan
