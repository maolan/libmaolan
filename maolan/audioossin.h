/* vim: set syntax=cpp: */
#pragma once
#include <string>
#include <maolan/audiooss.h>


namespace maolan
{
  namespace audio
  {
    class OSSIn: public OSS
    {
      public:
        OSSIn(const std::string &device, const std::size_t &channels);

        void fetch();
        void process();
    };
  }
}
