#include <sndfile.hh>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "audiooss.h"


int main(int argc, char **argv)
{
  if (argc < 2)
  {
    cerr << "Usage: " << argv[0] << " <wav file>" << endl;
    return 1;
  }
  const string filePath = argv[1];
  ifstream f(filePath);
  if (!f.good())
  {
    cerr << "Could not open file " << filePath << endl;
    return 1;
  }
  cout << filePath << endl;
  AudioOSSEngine engine;
  SndfileHandle audioFile{filePath};
  int bytesRead;
  int bufferSize = 128;
  int arr[bufferSize];
  while ((bytesRead = audioFile.read(arr, bufferSize)) != -1)
  {
    for (int i = 0; i < bytesRead; ++i)
    {
      int channel = i % 2;
      engine.push(channel, arr[i]);
    }
  }
  engine.sync();
  return 0;
}

