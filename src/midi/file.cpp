#include <fstream>
#include <maolan/midi/buffer.h>
#include <maolan/midi/event.h>
#include <maolan/midi/file.h>


using namespace maolan::midi;


static unsigned int readVarLen(std::fstream &file)
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
  return value;
}


static unsigned int bigEndianInt(std::fstream &file, int size)
{
  char rawData[size];
  int shift;
  unsigned int result = 0;
  file.read(rawData, size);
  for (int i = 0; i < size; ++i)
  {
    shift = (size - 1 - i) * 8;
    result |= rawData[i] << shift;
  }
  return result;
}


static void readMetaEvent(std::fstream &file, Buffer chunk)
{
  file >> chunk->meta;
  auto len = readVarLen(file);
  chunk->data = new unsigned char[len];
  file.read((char *)chunk->data, len);
}


File::File(const std::string &path) : file(path) {}


File::~File() { file.close(); }


Buffer File::read()
{
  Buffer chunk = std::make_shared<BufferData>();
  chunk->time = readVarLen(file) / rate;
  if (last != nullptr)
  {
    chunk->time += last->time;
  }
  if (!file.good())
  {
    throw std::invalid_argument("Error reading event time!");
  }
  file >> chunk->type;
  if (!file.good())
  {
    throw std::invalid_argument("Error reading event type!");
  }
  switch (chunk->type)
  {
    case Event::META:
      readMetaEvent(file, chunk);
      if (!file.good())
      {
        throw std::invalid_argument("Error reading meta event!");
      }
      break;
    case Event::NOTE_ON:
    case Event::NOTE_OFF:
      file >> chunk->note >> chunk->velocity;
      if (!file.good())
      {
        throw std::invalid_argument("Error reading note event!");
      }
      break;
    case Event::CONTROLER_ON:
      file >> chunk->controller >> chunk->value;
      if (!file.good())
      {
        throw std::invalid_argument("Error reading controller event!");
      }
      break;
  }
  last = chunk;
  return chunk;
}


void File::readHeaders()
{
  int size = 4;
  char rawData[size + 1];

  // MThd
  file.read(rawData, size);
  rawData[size] = '\0';
  if (std::strncmp(rawData, "MThd", size) != 0)
  {
    throw std::invalid_argument("Not a MIDI file!");
  }
  headerLength = bigEndianInt(file, 4);
  if (!file.good())
  {
    throw std::invalid_argument("Error reading header length!");
  }
  format = bigEndianInt(file, 2);
  if (!file.good())
  {
    throw std::invalid_argument("Error reading format from header!");
  }
  chunks = bigEndianInt(file, 2);
  if (!file.good())
  {
    throw std::invalid_argument("Error reading chunks from header!");
  }
  division = bigEndianInt(file, 2);
  if (!file.good())
  {
    throw std::invalid_argument("Error reading division from header!");
  }
  rate = (float)division / (float)Config::division;

  // MTrk
  file.read(rawData, size);
  if (std::strncmp(rawData, "MTrk", size) != 0)
  {
    throw std::invalid_argument("Expected track marker!");
  }
  length = bigEndianInt(file, 4);
  if (!file.good())
  {
    throw std::invalid_argument("Error reading track length!");
  }
}


bool File::eof() { return file.eof(); }
bool File::good() { return file.good(); }
std::streampos File::tellg() { return file.tellg(); }
