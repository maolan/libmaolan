/* vim: set syntax=cpp: */
#pragma once
#include <string>
#include <vector>
#include <sndfile.hh>
#include <maolan/audioio.h>


class AudioFileInput : public AudioIO
{
  public:
    AudioFileInput(const std::string &path);
    ~AudioFileInput();

    void fetch();
    void split();
    void process();
    std::size_t channels() const;

  protected:
    SndfileHandle audioFile;

  private:
    float *rawData;
};
