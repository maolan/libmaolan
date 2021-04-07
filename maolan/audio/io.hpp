#pragma once
#include <nlohmann/json.hpp>
#include <vector>

#include "maolan/audio/buffer.hpp"
#include "maolan/config.hpp"
#include "maolan/io.hpp"


namespace maolan::audio
{
class IO : public maolan::IO
{
public:
  IO(const std::string &name, const bool &reg = false, const size_t &outs = 0);

  static std::vector<maolan::Config *> devices;

  virtual std::size_t channels() const;
  virtual Buffer pull(const std::size_t &channel = 0);
  virtual nlohmann::json json();

protected:
  std::vector<audio::Buffer> _outputs;
};
} // namespace maolan::audio
