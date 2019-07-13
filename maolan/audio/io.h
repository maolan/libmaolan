/* vim: set syntax=cpp: */
#pragma once
#include <string>
#include <vector>
#include <maolan/io.h>
#include <maolan/audio/chunk.h>


namespace maolan
{
  namespace audio
  {
    class IO : public maolan::IO
    {
      public:
        IO(const size_t &outs = 0, bool front = false);
        ~IO();

        virtual std::size_t channels() const = 0;
        Chunk pull(const unsigned &channel);
        void connect(IO *to);
        void connect(IO *to, std::size_t inCh, std::size_t outCh);

      protected:
        std::vector<Chunk> outputs;
    };
  }
}
