/* vim: set syntax=cpp: */
#pragma once
#include <maolan/audio/buffer.h>
#include <maolan/audio/connection.h>
#include <vector>


namespace maolan
{
namespace audio
{
class Input : public IO
{
public:
  Input();

  void add(IO *to, std::size_t ch);
  void fetch();
  void process();
  std::size_t channels() const;
  Buffer pull();

protected:
  std::vector<Connection> connections;
};
} // namespace audio
} // namespace maolan
