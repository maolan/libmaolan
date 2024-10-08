#pragma once
#include <nlohmann/json.hpp>
#include <vector>

#include <maolan/audio/buffer.hpp>
#include <maolan/config.hpp>
#include <maolan/io.hpp>

namespace maolan::audio {
class Input;
class Output;
class IO : public maolan::IO {
public:
  IO(const std::string &name, const bool &reg = true, const size_t &outs = 0);

  virtual void fetch();
  virtual void process();
  virtual bool leaf();
  virtual std::size_t channels() const;
  virtual Buffer pull(const std::size_t &channel = 0);
  virtual nlohmann::json json();
  virtual nlohmann::json connections();
  void connect(audio::IO *to);
  void connect(audio::IO *to, size_t inch, size_t outch);
  void backref(audio::IO *to, size_t inch, size_t outch);

protected:
  std::vector<Input *> _inputs;
  std::vector<Output *> _outputs;
};
} // namespace maolan::audio
