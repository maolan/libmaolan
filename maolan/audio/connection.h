/* vim: set syntax=cpp: */
#pragma once
#include <maolan/audio/io.h>


namespace maolan
{
  namespace audio
  {
    class Connection
    {
      public:
        Connection();
        Connection(IO *connectTo, std::size_t &ch);

        IO * get();
        void target(IO *connectTo, std::size_t &ch);
        Chunk pull();

      protected:
        IO *to;
        std::size_t channel;
    };
  }
}
