#include <maolan/audioconnection>


using namespace std;


AudioConnection::AudioConnection()
{
}


AudioConnection::AudioConnection(AudioIO *connectTo, size_t &ch)
{
  target(connectTo, ch);
}


void AudioConnection::target(AudioIO *connectTo, size_t &ch)
{
  to = connectTo;
  channel = ch;
}


AudioChunk AudioConnection::pull()
{
  return to->pull(channel);
}


AudioIO * AudioConnection::get()
{
  return to;
}
