#include <maolan/audiochunk.h>

using namespace maolan::audio;

AudioChunkData::AudioChunkData(const size_t &size)
{
  data = new float[size];
}


AudioChunkData::~AudioChunkData()
{
  delete []data;
}
