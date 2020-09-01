#include <maolan/midi/connectable.h>


using namespace maolan::midi;


Connectable::Connectable(const std::size_t &chs) { inputs.resize(chs); }


void Connectable::connect(IO *to)
{
  for (auto channel = 0; channel < inputs.size(); ++channel)
  {
    connect(to, channel, channel);
  }
}


void Connectable::connect(IO *to, std::size_t inCh, std::size_t outCh)
{
  inputs[inCh].connect(to, outCh);
}


void Connectable::fetch()
{
  for (auto &input : inputs)
  {
    input.fetch();
  }
}
void Connectable::process()
{
  for (auto &input : inputs)
  {
    input.process();
  }
}
