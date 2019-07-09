/* vim: set syntax=cpp: */
#pragma once
#include <vector>
#include <maolan/audioconnection.h>
#include <maolan/audiochunk.h>


namespace maolan
{
  namespace audio
  {
    class Input : public IO
    {
      public:
        Input();

        void add(IO *to, std::size_t ch);
        void fetch();
        void process();
        std::size_t channels() const;
        Chunk pull();

      protected:
        std::vector<Connection> connections;
    };
  }
}
