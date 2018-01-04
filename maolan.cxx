#include <sndfile.hh>
#include <iostream>
#include <chrono>
#include <thread>
#include "audiooss.h"


int main()
{
  const string file = "/usr/home/meka/Files/reporter44k1.wav";
  AudioOSSEngine engine;
  SndfileHandle audioFile{file};
  int bytesRead;
  int bufferSize = 128;
  int arr[bufferSize];
  while ((bytesRead = audioFile.read(arr, bufferSize)) != -1)
  {
    for (int i = 0; i < bytesRead; ++i)
    {
      Sample sample;
      int remain = i % 2;
      int arrayIndex = 2 * i + remain;
      sample.data = arr[arrayIndex];
      sample.channel = remain;
      engine.push(sample);
      cout << i << endl;
    }
  }
  engine.sync();
  return 0;
}

