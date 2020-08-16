#pragma once
#include <maolan/midi/buffer.h>
#include <maolan/midi/connection.h>
#include <vector>


namespace maolan::midi
{
class Input : public IO
{
public:
  Input();

  virtual void fetch();
  virtual void process();
  void connect(IO *to);
  std::size_t channels() const;
  Buffer pull();

protected:
  std::vector<Connection> connections;
};
} // namespace maolan::audio

