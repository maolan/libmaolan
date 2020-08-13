#include <fstream>
#include <iostream>
#include <cstring>
#include <maolan/midi/buffer.h>
#include <maolan/midi/event.h>
#include <maolan/midi/file.h>
#include <unistd.h>


unsigned int readVarLen(std::ifstream &file)
{
  unsigned int value;
  unsigned char c;
  file >> c;
  value = c;
  if (value & 0x80)
  {
    value &= 0x7F;
    do
    {
      value = (value << 7);
      file >> c;
      value += c & 0x7F;
    } while (c & 0x80);
  }
  return (value);
}


int bigEndianInt(std::ifstream &file, int size, bool debug = false)
{
  char rawData[size];
  int shift;
  int result = 0;
  file.read(rawData, size);
  for (int i = 0; i < size; ++i)
  {
    shift = (size - 1 - i) * 8;
    result |= rawData[i] << shift;
  }
  return result;
}


void readMetaEvent(std::ifstream &file, maolan::midi::BufferData *chunk)
{
  unsigned char type;
  file >> type;
  unsigned int len = readVarLen(file);
  chunk->data = new char[len];
  file.read(chunk->data, len);
}


void readNote(std::ifstream &file, maolan::midi::BufferData *chunk)
{
  file >> chunk->note >> chunk->velocity;
}


maolan::midi::MIDIFile::MIDIFile(const std::string &path) : file(path, std::ios::binary) {}


maolan::midi::MIDIFile::~MIDIFile() { file.close(); }


maolan::midi::BufferData * maolan::midi::MIDIFile::read()
{
  BufferData *chunk = new BufferData;
  chunk->channel = 0;
  chunk->note = 0;
  chunk->velocity = 0;
  chunk->data = nullptr;
  chunk->time = readVarLen(file);
  file >> chunk->type;
  switch (chunk->type)
  {
    case MIDIEvent::META:
      readMetaEvent(file, chunk);
      break;
    case MIDIEvent::NOTE_ON:
    case MIDIEvent::NOTE_OFF:
      readNote(file, chunk);
      break;
  }
  return chunk;
}


void maolan::midi::MIDIFile::skipHeaders()
{
  int size = 4;
  char rawData[size + 1];

  // MThd
  file.read(rawData, size);
  rawData[size] = '\0';
  if (std::strncmp(rawData, "MThd", size) != 0)
  {
    std::cerr << "Not a MIDI file" << std::endl;
  }
  int headerLength = bigEndianInt(file, 4);
  int format = bigEndianInt(file, 2);
  int chunks = bigEndianInt(file, 2);
  division = bigEndianInt(file, 2);

  // MTrk
  file.read(rawData, size);
  int length = bigEndianInt(file, 4);
}


bool maolan::midi::MIDIFile::eof() { return file.eof(); }
