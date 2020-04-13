#include <iostream>
#include <algorithm>
#include <maolan/config.h>
#include <maolan/audio/plugin.h>
#include <maolan/audio/pluginport.h>


using namespace maolan::audio;


static LV2_URID uri_to_id (LV2_URID_Map_Handle unused, const char* uri)
{
  return 4;
}


static const char * id_to_uri (LV2_URID_Unmap_Handle unused, LV2_URID urid)
{
  return "http://lv2plug.in/plugins/eg-amp";
}


LilvWorld *Plugin::world = nullptr;
LilvPlugins *Plugin::plugins = nullptr;


Plugin::Plugin(const std::string &argUri)
  : _identifier{argUri}
{
  if (world == nullptr)
  {
    world = lilv_world_new();
    lilv_world_load_all(world);
    plugins = (LilvPlugins *)lilv_world_get_all_plugins(world);
  }
  _uri = lilv_new_uri(world, _identifier.data());
  rawPlugin = lilv_plugins_get_by_uri(plugins, _uri);
  if (rawPlugin != nullptr)
  {
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
    lilv_plugin_get_port_ranges_float(rawPlugin, mins, maxes, defaults);
    for (uint32_t index = 0; index < num_ports; ++index) {
      const LilvPort *p = lilv_plugin_get_port_by_index(rawPlugin, index);
      PluginPort *port = new PluginPort(
        world,
        rawPlugin,
        p,
        mins[index],
        maxes[index],
        defaults[index],
        index
      );
      if (port->direction() == PluginPortDirection::input)
      {
        if (port->type() == PluginPortType::midi)
        {
          input.midi.push_back(port);
        }
        else if (port->type() == PluginPortType::audio)
        {
          input.audio.push_back(port);
        }
        else if (port->type() == PluginPortType::control)
        {
          input.control.push_back(port);
        }
      }
      else if (port->direction() == PluginPortDirection::output)
      {
        if (port->type() == PluginPortType::midi)
        {
          output.midi.push_back(port);
        }
        else if (port->type() == PluginPortType::audio)
        {
          output.audio.push_back(port);
        }
        else if (port->type() == PluginPortType::control)
        {
          output.control.push_back(port);
        }
      }
    }
  }
  else
  {
    std::cerr << "No such plugin " << _identifier << std::endl;
  }
	instance = lilv_plugin_instantiate(rawPlugin, Config::samplerate, nullptr);
	lilv_instance_activate(instance);
}


void Plugin::print() const
{
  if (rawPlugin == nullptr)
  {
    std::cerr << "No such plugin " << _identifier << std::endl;
    return;
  }
  std::cout << _name << ": " << _identifier << "\n";
  std::cout << "Author: " << _author.name;
  if (_author.email.size() > 0)
  {
    std::cout << " <" << _author.email << '>';
  }
  std::cout << ", " << _author.homepage << "\n\n";

  if (input.midi.size() > 0)
  {
    std::cout << "Input MIDI ports:" << '\n';
    for (const auto &port : input.midi) { port->print(); }
    std::cout << '\n';
  }
  if (input.audio.size() > 0)
  {
    std::cout << "Input audio ports:" << '\n';
    for (const auto &port : input.audio) { port->print(); }
    std::cout << '\n';
  }
  if (input.control.size() > 0)
  {
    std::cout << "Input control ports:" << '\n';
    for (const auto &port : input.control) { port->print(); }
    std::cout << '\n';
  }
  if (output.midi.size() > 0)
  {
    std::cout << "Output MIDI ports:" << '\n';
    for (const auto &port : output.midi) { port->print(); }
    std::cout << '\n';
  }
  if (output.audio.size() > 0)
  {
    std::cout << "Output audio ports:" << '\n';
    for (const auto &port : output.audio) { port->print(); }
    std::cout << '\n';
  }
  if (output.control.size() > 0)
  {
    std::cout << "Output control ports:" << '\n';
    for (const auto &port : output.control) { port->print(); }
    std::cout << '\n';
  }
}


const maolan::Frame * const Plugin::process(const maolan::Frame * const inputs)
{
  auto size = input.control.size();
  auto &controlResult  = input.control;
  auto &controlBuffer  = inputs->controls;
  for (uint32_t i = 0; i < size; ++i)
  {
    controlResult[i]->buffer(instance, controlBuffer[i]);
  }
  size = input.audio.size();
  auto &inputResult = input.audio;
  auto &inputBuffer = inputs->audioBuffer;
  for (uint32_t i = 0; i < size; ++i)
  {
    inputResult[i]->buffer(instance, inputBuffer[i]);
  }
  size = output.audio.size();
  auto outputs = new maolan::Frame(size, 0);
  auto &outputResult = output.audio;
  auto &outputBuffer = outputs->audioBuffer;
  for (uint32_t i = 0; i < size; ++i)
  {
    outputBuffer[i] = outputResult[i]->buffer(instance);
  }
  lilv_instance_run(instance, Config::audioBufferSize);
  return outputs;
}


const PluginInfo Plugin::info() const
{
  PluginInfo info;
  info.input.audio = input.audio.size();
  info.input.control = input.control.size();
  info.input.midi = input.midi.size();
  info.output.audio = output.audio.size();
  info.output.control = output.control.size();
  info.output.midi = output.midi.size();
  return info;
}


void Plugin::saveSession() const
{
  LV2_URID_Map uridMap = { NULL, &uri_to_id };
  LV2_URID_Unmap uridUnmap = { NULL, &id_to_uri };
  auto state = lilv_state_new_from_world(world, &uridMap, _uri);
  // auto result = lilv_state_save(world, &uridMap, &uridUnmap, state, _identifier.data(), "/tmp", "amp.ttl");
  // std::cout << result << '\n';
}


Plugin::~Plugin()
{
	lilv_instance_deactivate(instance);
  lilv_node_free((LilvNode *)_uri);
  lilv_instance_free(instance);
}


void Plugin::destroyWorld() { lilv_world_free(world); }
void Plugin::uri(const LilvNode *argUri) { _uri = argUri; }
const LilvNode * Plugin::uri() const { return _uri; }
const std::string Plugin::identifier() const { return _identifier; }
const std::string Plugin::name() const { return _name; }
const Author Plugin::author() const { return _author; }
