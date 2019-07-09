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
    class AudioTrack : public AudioIO
    {
      public:
        AudioTrack(const std::size_t &size);
        ~AudioTrack();

        void connect(AudioIO *to);
        void fetch();
        std::size_t channels() const;
        void process();
        void addFile(const std::string &filePath);

      protected:
        std::vector<AudioInput> inputs;
        std::vector<AudioFileInput> files;
    };
  }
}
