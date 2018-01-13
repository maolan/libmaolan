#include <maolan/audioconnection>


using namespace std;


AudioConnection::AudioConnection(AudioIO *connectTo, size_t ch)
{
  to = connectTo;
  channel = ch;
}


AudioChunk AudioConnection::pull()
{
  return to->pull(channel);
}
