#include <iostream>
#include <maolan/midi/clip.h>
#include <maolan/midi/file.h>


std::vector<MIDIChunk *>::iterator nextChunk;


MIDIClip::MIDIClip()
  : IO()
{
}


MIDIClip::~MIDIClip()
{
  data.clear();
}


void MIDIClip::fetch()
{
}


void MIDIClip::process()
{
}


void MIDIClip::load(const std::string &filepath)
{
  MIDIFile file(filepath);
  file.skipHeaders();
  while (!file.eof())
  {
    data.push_back(file.read());
  }
  if (!data.empty())
  {
    nextChunk = data.begin();
  }
  else
  {
    nextChunk = data.end();
  }
}


MIDIChunk * MIDIClip::next()
{
  if (data.empty() || nextChunk == data.end())
  {
    return nullptr;
  }
  MIDIChunk *m = *nextChunk;
  ++nextChunk;
  return m;
}
