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

    void fetch();
    Package pull();
    std::vector<int> data();

  protected:
    float *rawData;
    std::size_t size;
    std::size_t position;
    SndfileHandle audioFile;
    Package package;
};
