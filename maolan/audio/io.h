#pragma once
#include <maolan/audio/buffer.h>
#include <maolan/config.h>
#include <maolan/io.h>
#include <vector>


namespace maolan::audio
{
class IO : public maolan::IO
{
public:
  IO(const size_t &outs = 0, const bool &front = true, const bool &reg = false,
     const std::string &name = "");

  static std::vector<maolan::Config *> devices;

  virtual std::size_t channels() const;
  virtual Buffer pull(const std::size_t &channel);

protected:
  std::vector<audio::Buffer> outputs;
};
} // namespace maolan::audio
