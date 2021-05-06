#include "maolan/audio/alsa/base.hpp"


static void checkError(int &value, const std::string &message)
{
  if (value != 0)
  {
    throw std::invalid_argument(message);
  }
}


using namespace maolan::audio;
