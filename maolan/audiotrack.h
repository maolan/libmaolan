/* vim: set syntax=cpp: */
#pragma once
#include <string>
#include <maolan/audioio.h>
#include <maolan/audioinput.h>
#include <maolan/audiofileinput.h>


namespace maolan
{
  namespace audio
  {
    class Track : public IO
    {
      public:
        Track(const std::size_t &size);
        ~Track();

        void connect(IO *to);
        void fetch();
        std::size_t channels() const;
        void process();
        void addFile(const std::string &filePath);

      protected:
        std::vector<Input> inputs;
        std::vector<FileInput> files;
    };
  }
}
