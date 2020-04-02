/* vim: set syntax=cpp: */
#pragma once
#include <maolan/audio/input.h>
#include <maolan/audio/oss.h>
#include <maolan/audio/connectable.h>
#include <string>


namespace maolan::audio
{
class OSSOut : public OSS, public Connectable
{
public:
  OSSOut(const std::string &device, const std::size_t &channels);

  void fetch();
  void process();
  void convertToRaw();
  void play(int *frame, std::size_t dataSize);
};
} // namespace maolan::audio
