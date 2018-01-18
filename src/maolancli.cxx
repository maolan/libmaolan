#include <iostream>
#include <maolan/audiofileinput>
#include <maolan/audioossout>
#include <maolan/audiotrack>


using namespace std;


int runTracks(int argc, char **argv)
{
  if (argc < 2)
  {
    cerr << "Usage: " << argv[0] << " <wav file>" << endl;
    return 1;
  }
  const string &filePath = argv[1];
  AudioTrack track(2);
  track.addFile(filePath);
  AudioOSSOut out(2);
  out.connect(&track);
  cout << "Playing ..." << flush;
  while (true)
  {
    for (auto &io : AudioIO::ios) {io->fetch();}
    for (auto &io : AudioIO::ios) {io->process();}
  }
  cout << endl;
}


int main(int argc, char **argv)
{
  return runTracks(argc, argv);
}
