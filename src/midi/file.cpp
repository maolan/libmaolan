#include <cstring>
#include <fstream>
#include <iostream>

#include "maolan/config.hpp"
#include "maolan/midi/buffer.hpp"
#include "maolan/midi/event.hpp"
#include "maolan/midi/file.hpp"


using namespace maolan::midi;


static void write(std::fstream &file, const std::uint8_t &value)
{
  file << value;
}


static void write(std::fstream &file, const std::uint16_t &value)
{
  union {
    std::uint8_t bytes[2];
    std::uint32_t value;
  } data;
  data.value = value;
  file << data.bytes[1];
  file << data.bytes[0];
}


static void write(std::fstream &file, const std::uint32_t &value)
{
  union {
    std::uint8_t bytes[4];
    std::uint32_t value;
  } data;
  data.value = value;
  file << data.bytes[3];
  file << data.bytes[2];
  file << data.bytes[1];
  file << data.bytes[0];
}


static void writeVarLen(std::fstream &file, std::uint32_t value)
{
  std::uint32_t buffer;
  buffer = value & 0x7F;

  while ((value >>= 7) > 0)
  {
    buffer <<= 8;
    buffer += ((value & 0x7F) | 0x80);
  }

  while (true)
  {
    file << (std::uint8_t)buffer;
    if (!file.good())
    {
      std::cerr << "Error: " << strerror(errno) << '\n';
    }
    if (buffer & 0x80)
    {
      buffer >>= 8;
    }
    else
    {
      break;
    }
  }
}


static std::uint32_t readVarLen(std::fstream &file)
{
  std::uint32_t value;
  std::uint8_t c;
  file.read((char *)&c, 1);
  value = c;
  if (value & 0x80)
  {
    value &= 0x7F;
    do
    {
      value <<= 7;
      file.read((char *)&c, 1);
      value += c & 0x7F;
    } while (c & 0x80);
  }
  return value;
}


static std::uint32_t bigEndianInt(std::fstream &file, int size)
{
  std::uint8_t rawData[size];
  int shift;
  std::uint32_t result = 0;
  file.read((char *)rawData, size);
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


File::File(const std::string &path)
  : file{path, std::ios::in | std::ios::binary}
{
  _name = path;
  _type = "MIDIFile";
  file >> std::noskipws;
}


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
  if (chunk->type == Event::META)
  {
    readMetaEvent(file, chunk);
    if (!file.good())
    {
      throw std::invalid_argument("Error reading meta event!");
    }
  }
  else
  {
    chunk->type &= Event::NOTE_MASK;
    switch(chunk->type)
    {
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
  std::uint32_t length = bigEndianInt(file, 4);
  if (!file.good())
  {
    throw std::invalid_argument("Error reading track length!");
  }
}


void File::save(Buffer buffer)
{
  if (buffer == nullptr || _name == "/tmp/clip.mid")
  {
    return;
  }
  Buffer lastSaved = std::make_shared<BufferData>();
  lastSaved->time = 0;
  std::fstream outfile;
  outfile.open("/tmp/clip.mid",
               std::ios::out | std::ios::trunc | std::ios::binary);
  outfile << "MThd";
  write(outfile, headerLength);
  write(outfile, format);
  write(outfile, (std::uint16_t)1);
  write(outfile, Config::division);
  outfile << "MTrk";

  auto lengthPosition = outfile.tellg();
  auto dataPosition = lengthPosition;
  dataPosition += 4;
  outfile.seekg(dataPosition);

  std::uint32_t length = 0;
  std::uint8_t typeChannel;
  for (Buffer chunk = buffer; chunk != nullptr; chunk = chunk->next)
  {
    writeVarLen(outfile, chunk->time - lastSaved->time);
    typeChannel = chunk->type;
    write(outfile, typeChannel);
    switch (chunk->type)
    {
      case Event::NOTE_ON:
      case Event::NOTE_OFF:
        write(outfile, chunk->note);
        write(outfile, chunk->velocity);
        break;
      case Event::CONTROLER_ON:
        write(outfile, chunk->controller);
        write(outfile, chunk->value);
        break;
    }
    lastSaved = chunk;
    ++length;
  }
  std::uint8_t trackEnd;
  trackEnd = 0;
  write(outfile, trackEnd);
  trackEnd = 0xFF;
  write(outfile, trackEnd);
  trackEnd = 0x2F;
  write(outfile, trackEnd);
  trackEnd = 0;
  write(outfile, trackEnd);

  outfile.seekg(lengthPosition);
  write(outfile, length);
  outfile.close();
}


bool File::eof() { return file.eof(); }
bool File::good() { return file.good(); }
std::streampos File::tellg() { return file.tellg(); }
void File::fetch() {}
void File::process() {}
