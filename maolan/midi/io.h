#pragma once
#include <maolan/io.h>
#include <maolan/midi/buffer.h>


namespace maolan::midi
{
class MIDIIO : public maolan::IO
{
public:
  MIDIIO(const std::string &name = "", const bool &front = true,
         const bool &reg = false);

  static std::vector<maolan::Config *> devices;

  virtual Buffer pull();

protected:
  Buffer output;
};
} // namespace maolan::midi
