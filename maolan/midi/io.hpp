#pragma once
#include "maolan/io.hpp"
#include "maolan/midi/buffer.hpp"


namespace maolan::midi
{
class Input;
class IO : public maolan::IO
{
public:
  IO(const std::string &name = "", const bool &reg = false);

  virtual std::size_t channels() const;
  virtual midi::Buffer pull(const std::size_t &channel);
  virtual nlohmann::json connections();
  void connect(midi::IO *to);
  void connect(midi::IO *to, std::size_t inch, std::size_t outch);

protected:
  std::vector<midi::Buffer> _outputs;
  std::vector<midi::Input *> _inputs;
};
} // namespace maolan::midi
