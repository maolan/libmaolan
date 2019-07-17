/* vim: set syntax=cpp: */
#pragma once
#include <maolan/audio/buffer.h>
#include <maolan/io.h>
#include <string>
#include <vector>


namespace maolan
{
namespace audio
{
class IO : public maolan::IO
{
public:
  IO(const size_t &outs = 0, const bool &front = true, const bool &reg = true);
  ~IO();

  virtual std::size_t channels() const = 0;
  virtual Buffer pull(const unsigned &channel);
  void connect(IO *to);
  void connect(IO *to, std::size_t inCh, std::size_t outCh);

protected:
  std::vector<Buffer> outputs;
};
} // namespace audio
} // namespace maolan
