#include "fileinput.h"
#include <iostream>


using namespace std;


FileInput::FileInput(const string &path)
  : audioFile(path)
  , size(128)
  , position(0)
{
  rawData = new float(size);
}


FileInput::~FileInput()
{
  delete []rawData;
}


Package FileInput::pull()
{
  if (position == 0)
  {
    cout << "Get new chunk" << endl;
  }
  auto package = newPackage();
  position = (position + 1) % size;
  return package;
}
