#include <iostream>
#include <maolan/midi/buffer.h>
#include <maolan/midi/clip.h>
#include <maolan/midi/file.h>


std::vector<maolan::midi::BufferData *>::iterator nextChunk;


maolan::midi::MIDIClip::MIDIClip() : IO() {}


maolan::midi::MIDIClip::~MIDIClip() { data.clear(); }


void
maolan::midi::MIDIClip::fetch()
{}


void
maolan::midi::MIDIClip::process()
{}


void
maolan::midi::MIDIClip::load(const std::string &filepath)
{
  maolan::midi::MIDIFile file(filepath);
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


maolan::midi::BufferData *
maolan::midi::MIDIClip::next()
{
  if (data.empty() || nextChunk == data.end())
  {
    return nullptr;
  }
  BufferData *m = *nextChunk;
  ++nextChunk;
  return m;
}
