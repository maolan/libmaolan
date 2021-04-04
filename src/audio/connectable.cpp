#include "maolan/audio/connectable.hpp"


using namespace maolan::audio;


Connectable::Connectable(const std::size_t &chs) { _inputs.resize(chs); }


void Connectable::connect(IO *to)
{
  for (auto channel = 0; channel < _inputs.size(); ++channel)
  {
    connect(to, channel, channel);
  }
}


void Connectable::connect(IO *to, std::size_t inCh, std::size_t outCh)
{
  _inputs[inCh].connect(to, outCh);
}


void Connectable::fetch()
{
  for (auto &input : _inputs)
  {
    input.fetch();
  }
}


void Connectable::process()
{
  for (auto &input : _inputs)
  {
    input.process();
  }
}
