#include <iostream>
#include <maolan/audio/plugin.h>
#include <maolan/audio/pluginport.h>


using namespace maolan::audio;


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
  rawPlugin = (LilvPlugin *)lilv_plugins_get_by_uri(plugins, _uri);
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
        defaults[index]
      );
      if (port->direction() == PluginPortDirection::input)
      {
        if (port->type() == PluginPortType::atom)
        {
          input.atom.push_back(port);
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
        if (port->type() == PluginPortType::atom)
        {
          output.atom.push_back(port);
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

  if (input.atom.size() > 0)
  {
    std::cout << "Input atom ports:" << '\n';
    for (const auto &port : input.atom) { port->print(); }
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
  if (output.atom.size() > 0)
  {
    std::cout << "Output atom ports:" << '\n';
    for (const auto &port : output.atom) { port->print(); }
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


Plugin::~Plugin() { lilv_node_free((LilvNode *)_uri); }
void Plugin::destroyWorld() { lilv_world_free(Plugin::world); }
void Plugin::uri(const LilvNode *argUri) { _uri = argUri; }
const LilvNode * Plugin::uri() const { return _uri; }
const std::string Plugin::identifier() const { return _identifier; }
const std::string Plugin::name() const { return _name; }
const Author Plugin::author() const { return _author; }
