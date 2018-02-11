#include <maolan/audioconnection>


using namespace std;


vector<AudioConnection *> AudioConnection::connections;


AudioConnection::AudioConnection(AudioIO *connectTo, size_t ch)
{
  it = connections.emplace(connections.end(), this);
  to = connectTo;
  channel = ch;
}


AudioConnection::~AudioConnection()
{
  connections.erase(it);
}


AudioChunk AudioConnection::pull()
{
  return to->pull(channel);
}
