/* vim: set syntax=cpp: */
#pragma once
#include <memory>
#include <vector>
#include <maolan/bpm.h>


namespace maolan
{
  class Config
  {
    public:
      static std::size_t audioChunkSize;
      static int samplerate;
      static int bpm;
      static std::vector<BPM> bpms;
  };
}
