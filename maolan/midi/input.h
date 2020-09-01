#pragma once
#include <maolan/midi/buffer.h>
#include <maolan/midi/connection.h>
#include <vector>


namespace maolan::midi
{
class Input : public midi::IO
{
public:
  Input();

  virtual void fetch();
  virtual void process();
  void connect(IO *to, const std::size_t &ch = 0);
  Buffer pull();

protected:
  std::vector<midi::Connection> connections;
};
} // namespace maolan::midi

