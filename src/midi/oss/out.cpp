#include <iomanip>
#include <maolan/constants.h>
#include <maolan/midi/event.h>
#include <maolan/midi/oss/out.h>
#include <unistd.h>


using namespace maolan::midi;


OSSMIDIOut::OSSMIDIOut(const std::string &device) : OSSMIDI(device)
{
  _type = "OSSMIDIOut";
  _name = "OSS MIDI Out";
}


void OSSMIDIOut::fetch()
{
  Connectable::fetch();
}


void OSSMIDIOut::process() {}
