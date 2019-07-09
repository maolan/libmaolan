/* vim: set syntax=cpp: */
#pragma once
#include <string>
#include <vector>
#include <sndfile.hh>
#include <maolan/audioio.h>


namespace maolan
{
  namespace audio
  {
    class FileInput : public IO
    {
      public:
        FileInput(const std::string &path);
        ~FileInput();

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
