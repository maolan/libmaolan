#pragma once
#include "maolan/io.hpp"
#include "maolan/midi/buffer.hpp"


namespace maolan::midi
{
class IO : public maolan::IO
{
public:
  IO(const std::string &name = "", const bool &front = false,
     const bool &reg = false);

  static std::vector<maolan::Config *> devices;

  virtual std::size_t channels() const;
  virtual midi::Buffer pull(const std::size_t &channel);

protected:
  std::vector<midi::Buffer> _outputs;
};
} // namespace maolan::midi
