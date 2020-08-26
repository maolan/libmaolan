#pragma once
#include <maolan/io.h>
#include <maolan/midi/buffer.h>


namespace maolan::midi
{
class IO : public maolan::IO
{
public:
  IO(const std::string &name = "", const bool &front = true,
     const bool &reg = false);

  static std::vector<maolan::Config *> devices;

  virtual std::size_t channels() const;
  virtual midi::Buffer pull(const std::size_t &channel);

protected:
  std::vector<midi::Buffer> outputs;
};
} // namespace maolan::midi
