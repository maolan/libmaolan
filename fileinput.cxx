#include <iostream>
#include <limits>
#include "fileinput.h"


using namespace std;


FileInput::FileInput(const string &path)
  : position(0)
  , audioFile(path)
{
  size = 128 * audioFile.channels();
  rawData = new float(size);
}


FileInput::~FileInput()
{
  delete []rawData;
}


void FileInput::fetch()
{
  package = newPackage();
  auto buffer = package->second;
  int bytesRead = audioFile.read(rawData, size);
  for (int i = 0; i < bytesRead; ++i)
  {
    buffer->push_back(rawData[i]);
  }
}


Package FileInput::pull()
{
  return package;
}


vector<int> FileInput::data()
{
  vector<int> data;
  Package package = pull();
  auto buffer = package->second;
  for (auto &element : *buffer)
  {
    int result = element * numeric_limits<int>::max();
    data.push_back(result);
  }
  return data;
}
