#include <iostream>
#include "fileinput.h"


using namespace std;


int main(int argc, char **argv)
{
  FileInput fileInput("/usr/home/meka/Files/reporter44k1.wav");
  for (unsigned i = 0; i < 1024; ++i)
  {
    auto package = fileInput.pull();
  }
  return 0;
}
