/* vim: set syntax=cpp: */
#pragma once
#include <maolan/bpm.h>
#include <memory>
#include <vector>


namespace maolan
{
class Config
{
public:
  static std::size_t audioBufferSize;
  static int samplerate;
  static int bpm;
  static std::vector<BPM> bpms;
};
} // namespace maolan
