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
  BufferData();

  unsigned int controler;
  unsigned int channel;
  unsigned int value;
  unsigned int time;
  unsigned int note;
  unsigned int type;
  unsigned int velocity;
  char *data;
  Buffer next;

  void print();
};
} // namespace maolan::midi

