/* vim: set syntax=cpp: */
#pragma once
#include <maolan/audio/buffer.h>
#include <maolan/audio/connection.h>
#include <vector>


namespace maolan::audio
{
class Input : public IO
{
public:
  Input();

  void fetch();
  void process();
  void connect(IO *to, const std::size_t &ch = 0);
  std::size_t channels() const;
  Buffer pull();

protected:
  std::vector<Connection> connections;
};
} // namespace maolan::audio
