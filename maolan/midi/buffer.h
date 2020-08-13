#pragma once
#include <memory>
#include <vector>

namespace maolan::midi
{
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
  BufferData *next;

  void print();
};
typedef std::shared_ptr<BufferData> Buffer;
} // namespace maolan::midi

