#pragma once
#include <string>
#include <sndfile.hh>
#include "types.h"


class FileInput
{
  public:
    FileInput(const std::string &path);
    ~FileInput();

    Package pull();

  protected:
    float *rawData;
    std::size_t size;
    std::size_t position;
    SndfileHandle audioFile;
};
