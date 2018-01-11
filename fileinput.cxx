#include <iostream>
#include <deque>
#include <limits>
#include "fileinput.h"


using namespace std;


size_t FileInput::size = 0;


FileInput::FileInput(const string &path)
  : audioFile(path)
{
  for (int i = 0; i < audioFile.channels(); ++i)
  {
    channels.emplace_back(new deque<float>);
  }
  rawData = new float[audioFile.channels() * size];
}


FileInput::~FileInput()
{
  delete []rawData;
}


void FileInput::fetch()
{
  int bytesRead = audioFile.read(rawData, audioFile.channels() * size);
  for (int i = 0; i < bytesRead; ++i)
  {
    channels[i % audioFile.channels()]->push_back(rawData[i]);
  }
  for (int i = bytesRead; i < audioFile.channels() * size; ++i)
  {
    channels[i % audioFile.channels()]->push_back(0);
  }
}


Chunk FileInput::pull(const unsigned &channel)
{
  return channels[channel];
}
