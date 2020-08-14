#pragma once
#include <maolan/midi/buffer.h>
#include <maolan/midi/connection.h>
#include <vector>


namespace maolan::midi
{
class Input : public MIDIIO
{
public:
  Input();

  virtual void fetch();
  virtual void process();
  void connect(MIDIIO *to);
  std::size_t channels() const;
  Buffer pull();

protected:
  std::vector<Connection> connections;
};
} // namespace maolan::audio

