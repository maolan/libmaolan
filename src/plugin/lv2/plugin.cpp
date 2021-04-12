#include "maolan/plugin/lv2/plugin.hpp"
#include "maolan/config.hpp"
#include "maolan/plugin/lv2/port.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <lv2/lv2plug.in/ns/ext/state/state.h>


namespace audio = maolan::audio;
namespace midi = maolan::midi;
using namespace maolan::plugin::lv2;


LilvWorld *Plugin::world = nullptr;
LilvPlugins *Plugin::plugins = nullptr;
audio::Buffer Plugin::emptyBuffer =
    std::make_shared<audio::BufferData>(Config::audioBufferSize);
float val = 0.0;


Plugin::Plugin(const std::string &argUri)
    : maolan::plugin::IO(argUri, true), _identifier{argUri}
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
    if (val)
    {
      _name = lilv_node_as_string(val);
    }
    val = lilv_plugin_get_author_name(rawPlugin);
    if (val)
    {
      _author.name = lilv_node_as_string(val);
    }
    val = lilv_plugin_get_author_homepage(rawPlugin);
    if (val)
    {
      _author.homepage = lilv_node_as_string(val);
    }
    val = lilv_plugin_get_author_email(rawPlugin);
    if (val)
    {
      _author.email = lilv_node_as_string(val);
    }
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
    for (uint32_t index = 0; index < num_ports; ++index)
    {
      const LilvPort *p = lilv_plugin_get_port_by_index(rawPlugin, index);
      PluginPort *port = new PluginPort(world, rawPlugin, p, mins[index],
                                        maxes[index], defaults[index], index);
      if (port->direction() == PluginPortDirection::input)
      {
        if (port->type() == PluginPortType::midi)
        {
          input.midi.push_back(port);
          midi::Connectable::_inputs.emplace_back();
        }
        else if (port->type() == PluginPortType::audio)
        {
          input.audio.push_back(port);
          audio::Connectable::_inputs.emplace_back();
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
          midi::IO::_outputs.emplace_back();
        }
        else if (port->type() == PluginPortType::audio)
        {
          output.audio.push_back(port);
          audio::IO::_outputs.emplace_back();
        }
        else if (port->type() == PluginPortType::control)
        {
          output.control.push_back(port);
        }
      }
    }
    instance = lilv_plugin_instantiate(rawPlugin, Config::samplerate, nullptr);
    lilv_instance_activate(instance);

    delete[] mins;
    delete[] maxes;
    delete[] defaults;
  }
  else
  {
    std::cerr << "No such plugin " << _identifier << std::endl;
  }
}


const void *Plugin::portValue(const char *port_symbol, void *user_data,
                              uint32_t *size, uint32_t *type)
{
  auto plugin = (Plugin *)user_data;
  assert(plugin != nullptr);
  std::cout << port_symbol << '\n';
  return nullptr;
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
    for (const auto &port : input.midi)
    {
      port->print();
    }
    std::cout << '\n';
  }
  if (input.audio.size() > 0)
  {
    std::cout << "Input audio ports:" << '\n';
    for (const auto &port : input.audio)
    {
      port->print();
    }
    std::cout << '\n';
  }
  if (input.control.size() > 0)
  {
    std::cout << "Input control ports:" << '\n';
    for (const auto &port : input.control)
    {
      port->print();
    }
    std::cout << '\n';
  }
  if (output.midi.size() > 0)
  {
    std::cout << "Output MIDI ports:" << '\n';
    for (const auto &port : output.midi)
    {
      port->print();
    }
    std::cout << '\n';
  }
  if (output.audio.size() > 0)
  {
    std::cout << "Output audio ports:" << '\n';
    for (const auto &port : output.audio)
    {
      port->print();
    }
    std::cout << '\n';
  }
  if (output.control.size() > 0)
  {
    std::cout << "Output control ports:" << '\n';
    for (const auto &port : output.control)
    {
      port->print();
    }
    std::cout << '\n';
  }
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
  // LilvState * const lilvState = lilv_state_new_from_instance(
  // state->synth->_handle,
  // state->handle,
  // &state->synth->_lv2_urid_map,
  // cPlugFileDirName,
  // cPresetDir,
  // cPresetDir,
  // cPresetDir,
  // LV2Synth::lv2state_getPortValue,
  // state,
  // LV2_STATE_IS_POD | LV2_STATE_IS_PORTABLE,
  // NULL
  // );
  std::string plugfiledirname;
  std::string presetdir;
  LilvState *const lilvState = lilv_state_new_from_instance(
      rawPlugin, instance, (LV2_URID_Map *)&_lv2_urid_map,
      plugfiledirname.data(), presetdir.data(), presetdir.data(),
      presetdir.data(), portValue, (void *)this,
      LV2_STATE_IS_POD | LV2_STATE_IS_PORTABLE, nullptr);
  std::cout << "lilvState = " << lilvState << '\n';
}


Plugin::~Plugin()
{
  lilv_instance_deactivate(instance);
  lilv_node_free((LilvNode *)_uri);
  lilv_instance_free(instance);
}


void Plugin::allocate()
{
  emptyBuffer = std::make_shared<audio::BufferData>(Config::audioBufferSize);
}


std::size_t Plugin::ports(const std::string &type, const std::string &direction)
{
  if (direction == "out")
  {
    if (type == "audio")
    {
      return output.audio.size();
    }
    if (type == "midi")
    {
      return output.midi.size();
    }
    if (type == "control")
    {
      return output.control.size();
    }
    std::cerr << "No such type: " << type << '\n';
    return 0;
  }
  if (direction == "in")
  {
    if (type == "audio")
    {
      return input.audio.size();
    }
    if (type == "midi")
    {
      return input.midi.size();
    }
    if (type == "control")
    {
      return input.control.size();
    }
    std::cerr << "No such type: " << type << '\n';
    return 0;
  }
  std::cerr << "No such direction!\n";
  return 0;
}


void Plugin::fetch()
{
  plugin::IO::fetch();
  auto portCount = ports("audio", "in");
  auto &audioins = audio::Connectable::_inputs;
  auto &iaudio = input.audio;
  for (std::size_t i = 0; i < portCount; ++i)
  {
    auto buffer = audioins[i].pull();
    if (buffer == nullptr)
    {
      iaudio[i]->buffer(instance, emptyBuffer);
    }
    else
    {
      iaudio[i]->buffer(instance, buffer);
    }
  }

  portCount = ports("midi", "in");
  auto &midiins = midi::Connectable::_inputs;
  auto &imidi = input.midi;
  for (std::size_t i = 0; i < portCount; ++i)
  {
    auto buffer = midiins[i].pull();
    imidi[i]->buffer(instance, buffer);
  }

  portCount = ports("audio", "out");
  auto &audioouts = audio::IO::_outputs;
  auto &oaudio = output.audio;
  for (std::size_t i = 0; i < portCount; ++i)
  {
    auto buffer = std::make_shared<audio::BufferData>(Config::audioBufferSize);
    oaudio[i]->buffer(instance, buffer);
    audioouts[i] = buffer;
  }

  portCount = ports("midi", "out");
  auto &midiouts = midi::IO::_outputs;
  auto &omidi = output.midi;
  for (std::size_t i = 0; i < portCount; ++i)
  {
    auto buffer = std::make_shared<midi::BufferData>();
    omidi[i]->buffer(instance, buffer);
    midiouts[i] = buffer;
  }
}


void Plugin::process() { lilv_instance_run(instance, Config::audioBufferSize); }


void Plugin::destroyWorld() { lilv_world_free(world); }
void Plugin::uri(const LilvNode *argUri) { _uri = argUri; }
const LilvNode *Plugin::uri() const { return _uri; }
const std::string Plugin::identifier() const { return _identifier; }
const std::string Plugin::name() const { return _name; }
const Author Plugin::author() const { return _author; }
