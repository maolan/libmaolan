/* vim: set syntax=cpp: */
#pragma once
#include <memory>


class Config
{
  public:
    static std::size_t audioChunkSize;
    static int samplerate;
    static int bpm;
};
