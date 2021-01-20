#include <iostream>
#include <sstream>
#include "maolan/plugin/instrument.hpp"


using namespace maolan::plugin;


Instrument::Instrument(const std::string &name, const std::string &type)
{
  if (type == "lv2")
  {
    std::stringstream s;
    _plugin = new lv2::Plugin(name);
    s << name << "-audio";
    _audio = new maolan::audio::Track(s.str(), _plugin->ports("audio", "out"));
    s.str("");
    s << name << "-midi";
    _midi = new maolan::midi::Track(s.str(), _plugin->ports("midi", "in"));
    _midi->arm();
    _plugin->connect(_midi);
    _audio->connect(_plugin);
  }
  else
  {
    std::cerr << "No such plugin type!\n";
  }
}


maolan::audio::Track *Instrument::audio() { return _audio; }
maolan::midi::Track *Instrument::midi() { return _midi; }
void Instrument::connect(maolan::audio::IO *to) { _audio->connect(to); }
void Instrument::connect(maolan::midi::IO *to) { _midi->connect(to); }
