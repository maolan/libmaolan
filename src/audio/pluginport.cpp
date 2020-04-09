#include <iostream>
#include <maolan/audio/pluginport.h>


using namespace maolan::audio;


LilvNode *PluginPort::lv2_AtomPort = nullptr;
LilvNode *PluginPort::lv2_AudioPort = nullptr;
LilvNode *PluginPort::lv2_ControlPort = nullptr;
LilvNode *PluginPort::lv2_CVPort = nullptr;
LilvNode *PluginPort::lv2_EventPort = nullptr;
LilvNode *PluginPort::lv2_InputPort = nullptr;
LilvNode *PluginPort::lv2_MidiEvent = nullptr;
LilvNode *PluginPort::lv2_OutputPort = nullptr;
LilvNode *PluginPort::lv2_ConnectionOptional = nullptr;


PluginPort::PluginPort(
  LilvWorld *world,
  const LilvPlugin *rawPlugin,
  const LilvPort *p,
  const float &argMinimum,
  const float &argMaximum,
  const float &argDdefault
)
{
  if (lv2_AtomPort == nullptr)
  {
    lv2_AtomPort = lilv_new_uri(world, LILV_URI_ATOM_PORT);
    lv2_AudioPort = lilv_new_uri(world, LILV_URI_AUDIO_PORT);
    lv2_ControlPort = lilv_new_uri(world, LILV_URI_CONTROL_PORT);
    lv2_CVPort = lilv_new_uri(world, LILV_URI_CV_PORT);
    lv2_EventPort = lilv_new_uri(world, LILV_URI_EVENT_PORT);
    lv2_InputPort = lilv_new_uri(world, LILV_URI_INPUT_PORT);
    lv2_MidiEvent = lilv_new_uri(world, LILV_URI_MIDI_EVENT);
    lv2_OutputPort = lilv_new_uri(world, LILV_URI_OUTPUT_PORT);
    lv2_ConnectionOptional = lilv_new_uri(world, LV2_CORE__connectionOptional);
  }
  rawPort = (LilvPort *)p;

  if (lilv_port_is_a(rawPlugin, rawPort, lv2_InputPort))
  {
    _direction = PluginPortDirection::input;
  }
  else if (lilv_port_is_a(rawPlugin, rawPort, lv2_OutputPort))
  {
    _direction = PluginPortDirection::output;
  }

  if (lilv_port_is_a(rawPlugin, rawPort, lv2_AudioPort))
  {
    _type = PluginPortType::audio;
  }
  else if (lilv_port_is_a(rawPlugin, rawPort, lv2_ControlPort))
  {
    _type = PluginPortType::control;
  }
  else if (lilv_port_is_a(rawPlugin, rawPort, lv2_AtomPort))
  {
    if (lilv_port_supports_event(rawPlugin, rawPort, lv2_MidiEvent))
    {
      _type = PluginPortType::midi;
    }
  }

	const LilvNode *sym = lilv_port_get_symbol(rawPlugin, rawPort);
	_symbol = lilv_node_as_string(sym);
	sym = lilv_port_get_name(rawPlugin, rawPort);
	_name = lilv_node_as_string(sym);
}


void PluginPort::print() const
{
  std::cout << "Port " << _name << '\n';
  std::cout << "\tSymbol " << _symbol<< '\n';
}


PluginPort::~PluginPort()
{
}
