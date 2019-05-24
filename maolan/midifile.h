#pragma once
#include <string>
#include <fstream>
#include <maolan/io.h>
#include <maolan/midichunk.h>


class MIDIFile
{
  public:
    MIDIFile(const std::string &path);
    ~MIDIFile();

    MIDIChunk * read();
    void skipHeaders();
    bool eof();

    int division;

  protected:
    std::ifstream file;
};
