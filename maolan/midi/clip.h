#pragma once
#include <maolan/io.h>
#include <maolan/midi/buffer.h>
#include <string>
#include <vector>


namespace maolan::midi
{
class MIDIClip : public maolan::IO
{
public:
  MIDIClip();
  ~MIDIClip();

  BufferData * next();
  void load(const std::string &filename);
  void fetch();
  void process();

protected:
  std::vector<BufferData *> data;
};
} // namespace maolan::midi
