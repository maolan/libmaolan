#pragma once
#include <memory>
#include <vector>

namespace maolan::midi {
class BufferData;
typedef std::shared_ptr<BufferData> Buffer;

class BufferData {
public:
  ~BufferData();

  std::uint8_t type;
  union {
    std::uint8_t note;
    std::uint8_t controller;
  };
  union {
    std::uint8_t velocity;
    std::uint8_t value;
  };
  std::uint8_t channel;
  std::uint8_t meta;
  std::size_t time;
  unsigned char *data = nullptr;
  Buffer next = nullptr;

  void print();
};
} // namespace maolan::midi
