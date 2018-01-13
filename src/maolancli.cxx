#include <iostream>
#include <maolan/audiofileinput>
#include <maolan/audioossout>


using namespace std;


int main(int argc, char **argv)
{
  if (argc < 2)
  {
    cerr << "Usage: " << argv[0] << " <wav file>" << endl;
    return 1;
  }
  AudioFileInput f(argv[1]);
  AudioOSSOut o(2);
  o.connect(&f);
  cout << "Playing ..." << flush;
  while (true)
  {
    // Fetch
    for (auto &io : AudioIO::ios) {io->fetch();}

    // Process
    for (auto &io : AudioIO::ios) {io->process();}
  }
  cout << endl;
}
