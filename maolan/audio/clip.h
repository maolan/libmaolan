#pragma once
#include <sndfile.hh>
#include <maolan/audio/io.h>
#include <maolan/audio/fileinput.h>
#include <cstdint>

namespace maolan
{
  namespace audio
  {
    class Clip : public IO
    {
      public:
        Clip(const uint64_t &offset = 0, const std::string &path = "");

        void fetch();
        void process();
        std::size_t channels() const;
        uint64_t offset();
        void offset(uint64_t offset);
        Chunk pull(const unsigned &channel);

      protected:
        uint64_t _offset;
        bool initialized= false;
        FileInput file;
    };
  }
}
