#pragma once
#include <vector>
#include <maolan/io.h>
#include <maolan/audio/buffer.h>


namespace maolan::audio
{
class IO : public maolan::IO
{
public:
  IO(const size_t &outs = 0, const bool &front = true, const bool &reg = true);

  virtual std::size_t channels() const = 0;
  virtual Buffer pull(const std::size_t &channel);

protected:
  std::vector<Buffer> outputs;
};
} // namespace maolan::audio
