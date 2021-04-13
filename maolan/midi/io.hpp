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

  static std::vector<maolan::Config *> devices;

  virtual void fetch();
  virtual void process();
  virtual std::size_t channels() const;
  virtual midi::Buffer pull(const std::size_t &channel);
  void connect(midi::IO *to);
  void connect(midi::IO *to, std::size_t inCh, std::size_t outCh);

protected:
  std::vector<midi::Buffer> _outputs;
  std::vector<midi::Input *> _inputs;
};
} // namespace maolan::midi
