#pragma once
#include <string>
#include <vector>
#include <sndfile.hh>
#include "types.h"


class FileInput
{
  public:
    FileInput(const std::string &path);
    ~FileInput();

    static std::size_t size;

    void fetch();
    Chunk pull(const unsigned &channel);

  protected:
    SndfileHandle audioFile;
    std::vector<Chunk> channels;


  private:
    float *rawData;
};
