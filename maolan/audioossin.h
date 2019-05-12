/* vim: set syntax=cpp: */
#pragma once
#include <string>
#include <maolan/audiooss.h>


class AudioOSSIn: public AudioOSS
{
  public:
    AudioOSSIn(const std::string &device, const std::size_t &channels);

    void fetch();
    void process();
};
