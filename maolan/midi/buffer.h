#pragma once
#include <memory>
#include <vector>


namespace maolan::midi
{
class BufferData;
typedef std::shared_ptr<BufferData> Buffer;

class BufferData
{
public:
  ~BufferData();

  std::uint8_t channel;
  std::uint8_t controller;
  std::uint8_t meta;
  std::uint8_t note;
  std::uint8_t velocity;
  std::uint8_t type;
  std::size_t time;
  std::uint8_t value;
  unsigned char *data = nullptr;
  Buffer next = nullptr;

  void print();
};
} // namespace maolan::midi
