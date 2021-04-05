#pragma once
#include <vector>
#include "maolan/midi/buffer.hpp"
#include "maolan/midi/connection.hpp"


namespace maolan::midi
{
class Input : public midi::IO
{
public:
  Input();

  virtual void fetch();
  virtual void process();
  void connect(IO *to, const std::size_t &ch = 0);
  Buffer pull(const std::size_t &channel = 0);

protected:
  std::vector<midi::Connection> connections;
};
} // namespace maolan::midi

