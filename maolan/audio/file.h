/* vim: set syntax=cpp: */
#pragma once
#include <string>
#include <vector>
#include <sndfile.hh>
#include <maolan/audio/io.h>


namespace maolan
{
  namespace audio
  {
    class File : public IO
    {
      public:
        File(const std::string &path = "");
        ~File();

        void fetch();
        void split();
        void process();
        std::size_t channels() const;

      protected:
        SndfileHandle audioFile;

      private:
        float *rawData;
    };
  }
}
