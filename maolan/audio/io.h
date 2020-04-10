#pragma once
#include <maolan/audio/buffer.h>
#include <maolan/io.h>
#include <vector>


namespace maolan::audio
{
class IO : public maolan::IO
{
public:
  IO(const size_t &outs = 0, const bool &front = true, const bool &reg = true);

  virtual std::size_t channels() const = 0;
  virtual Buffer pull(const unsigned &channel);

protected:
  std::vector<Buffer> outputs;
};
} // namespace maolan::audio
