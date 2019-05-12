#include <maolan/audiochunk.h>


AudioChunkData::AudioChunkData(const size_t &size)
{
  data = new float[size];
}


AudioChunkData::~AudioChunkData()
{
  delete []data;
}
