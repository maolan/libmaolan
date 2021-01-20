#pragma once
#include <vector>
#include "maolan/audio/buffer.hpp"
#include "maolan/audio/connection.hpp"


namespace maolan::audio
{
class Input : public audio::IO
{
public:
  Input();

  virtual void fetch();
  virtual void process();
  void connect(IO *to, const std::size_t &ch = 0);
  std::size_t channels() const;
  Buffer pull();

protected:
  std::vector<audio::Connection> connections;
};
} // namespace maolan::audio
