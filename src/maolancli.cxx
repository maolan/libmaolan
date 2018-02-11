#include <iostream>
#include <maolan/audiofileinput>
#include <maolan/audioossout>
#include <maolan/audioossin>
#include <maolan/audiotrack>
#include <maolan/config>


using namespace std;


int main(int argc, char **argv)
{
  AudioOSSIn in(2);
  AudioOSSOut out(2);
  out.connect(&in);
  cout << "Playing ..." << endl;
  while (true)
  {
    for (auto &io : AudioIO::ios)
    {
      io->fetch();
      io->process();
    }
  }
  cout << endl;
  return 0;
}
