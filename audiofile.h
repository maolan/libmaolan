#pragma once

#include <string>


using namespace std;


class AudioFile {
  public:
    AudioFile(const string &name = "");

    void play(const string &file) const;

  protected:
    const string name;
};
