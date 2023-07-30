#pragma once
#include <maolan/io.hpp>
#include <maolan/midi/buffer.hpp>

namespace maolan::midi {
class Input;
class IO : public maolan::IO {
public:
  IO(const std::string &name = "", const bool &reg = false);

  virtual void fetch();
  virtual void process();
  virtual midi::Buffer pull();
  virtual nlohmann::json connections();
  void connect(midi::IO *to);

protected:
  midi::Buffer _output;
  midi::Input *_input;
};
} // namespace maolan::midi
