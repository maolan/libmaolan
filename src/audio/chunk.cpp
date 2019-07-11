#include <maolan/audio/chunk.h>

using namespace maolan::audio;

ChunkData::ChunkData(const size_t &size)
{
  data = new float[size];
}


ChunkData::~ChunkData()
{
  delete []data;
}
