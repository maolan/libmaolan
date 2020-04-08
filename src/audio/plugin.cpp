#include <maolan/audio/plugin.h>


using namespace maolan::audio;

LilvNode *lv2_AtomPort;
LilvNode *lv2_AudioPort;
LilvNode *lv2_ControlPort;
LilvNode *lv2_CVPort;
LilvNode *lv2_EventPort;
LilvNode *lv2_InputPort;
LilvNode *lv2_MidiEvent;
LilvNode *lv2_OutputPort;
LilvNode *lv2_ConnectionOptional;

LilvWorld *Plugin::world = nullptr;
LilvPlugins *Plugin::plugins = nullptr;

void Plugin::destroyWorld() { lilv_world_free(Plugin::world); }


Plugin::Plugin(const std::string &argUri)
  : _identifier{argUri}
{
  if (world == nullptr)
  {
    world = lilv_world_new();
    lilv_world_load_all(world);
    plugins = (LilvPlugins *)lilv_world_get_all_plugins(world);
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
  uri(lilv_new_uri(world, _identifier.data()));
  rawPlugin = (LilvPlugin *)lilv_plugins_get_by_uri(plugins, _uri);
  auto val = lilv_plugin_get_name(rawPlugin);
  if (val) { _name = lilv_node_as_string(val); }
  val = lilv_plugin_get_author_name(rawPlugin);
  if (val) { _author.name = lilv_node_as_string(val); }
  val = lilv_plugin_get_author_homepage(rawPlugin);
  if (val) { _author.homepage = lilv_node_as_string(val); }
  val = lilv_plugin_get_author_email(rawPlugin);
  if (val) { _author.email = lilv_node_as_string(val); }
  if (_author.email.size() > 7)
  {
    _author.email = _author.email.substr(7, std::string::npos);
  }

  // Ger ports info
  uint32_t num_ports = lilv_plugin_get_num_ports(rawPlugin);
  float *mins = new float[num_ports];
  float *maxes = new float[num_ports];
  float *defaults = new float[num_ports];
	for (uint32_t index = 0; index < num_ports; ++index) {
    const LilvPort *port = lilv_plugin_get_port_by_index(rawPlugin, index);
    if (lilv_port_is_a(rawPlugin, port, lv2_InputPort))
    {
      std::cout << "Input ";
      if (lilv_port_is_a(rawPlugin, port, lv2_AudioPort))
      {
        std::cout << "audio";
      }
      else if (lilv_port_is_a(rawPlugin, port, lv2_ControlPort))
      {
        std::cout << "control";
      }
      else if (lilv_port_is_a(rawPlugin, port, lv2_AtomPort))
      {
        std::cout << "atom";
      }
    }
    else if (lilv_port_is_a(rawPlugin, port, lv2_OutputPort))
    {
      std::cout << "Output ";
      if (lilv_port_is_a(rawPlugin, port, lv2_AudioPort))
      {
        std::cout << "audio";
      }
      else if (lilv_port_is_a(rawPlugin, port, lv2_ControlPort))
      {
        std::cout << "control";
      }
      else if (lilv_port_is_a(rawPlugin, port, lv2_AtomPort))
      {
        std::cout << "atom";
      }
    }
    std::cout << '\n';
	}
}


Plugin::~Plugin()
{
  lilv_node_free((LilvNode *)_uri);
}


void Plugin::print() const
{
  std::cout << _name << ": " << _identifier << "\n";
  std::cout << "Author: " << _author.name << " <" << _author.email << ">, ";
  std::cout << _author.homepage << std::endl;
}


void Plugin::uri(const LilvNode *argUri) { _uri = argUri; }
const LilvNode * Plugin::uri() const { return _uri; }
const std::string Plugin::identifier() const { return _identifier; }
const std::string Plugin::name() const { return _name; }
const Author Plugin::author() const { return _author; }
