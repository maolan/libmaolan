/* vim: set syntax=cpp: */
#pragma once
#include <string>
#include <maolan/audiooss.h>
#include <maolan/audioinput.h>


namespace maolan
{
  namespace audio
  {
    class AudioOSSOut : public AudioOSS
    {
      public:
        AudioOSSOut(const std::string &device, const std::size_t &channels);

        void connect(AudioIO *to);
        void connect(AudioIO *to, std::size_t inCh, std::size_t outCh);
        void fetch();
        void process();
        void convertToRaw();
        void play(int *rawData, std::size_t dataSize);

      protected:
        std::vector<AudioInput> inputs;
    };
  }
}
