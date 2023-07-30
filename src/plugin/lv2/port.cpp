#include <iostream>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/forge.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>

#include <maolan/config.hpp>
#include <maolan/io.hpp>
#include <maolan/plugin/lv2/port.hpp>

using namespace maolan::plugin::lv2;

static std::uint32_t uid = 0;
struct Message {
  std::uint8_t *buf;
  LV2_Atom atom;
};

struct MIDINoteEvent {
  LV2_Atom_Event event;
  uint8_t *msg;
};

static uint32_t _map(void *data, const char *) {
  auto urid = (std::uint32_t *)data;
  return ++(*urid);
}

LilvNode *PluginPort::lv2_AtomPort = nullptr;
LilvNode *PluginPort::lv2_AudioPort = nullptr;
LilvNode *PluginPort::lv2_ControlPort = nullptr;
LilvNode *PluginPort::lv2_CVPort = nullptr;
LilvNode *PluginPort::lv2_EventPort = nullptr;
LilvNode *PluginPort::lv2_InputPort = nullptr;
LilvNode *PluginPort::lv2_MidiEvent = nullptr;
LilvNode *PluginPort::lv2_OutputPort = nullptr;
LilvNode *PluginPort::lv2_ConnectionOptional = nullptr;
LV2_URID_Map PluginPort::map = {.handle = &uid, .map = _map};

// static inline LV2_Atom_Event *
// _lv2_atom_sequence_append_event(LV2_Atom_Sequence *seq, uint32_t capacity,
// const LV2_Atom_Event *event)
// {
// const uint32_t total_size = (uint32_t)sizeof(*event) + event->body.size;
// if (capacity - seq->atom.size < total_size)
// {
// return nullptr;
// }
//
// LV2_Atom_Event *e = lv2_atom_sequence_end(&seq->body, seq->atom.size);
// memcpy(e, event, total_size);
// seq->atom.size += lv2_atom_pad_size(total_size);
// return e;
// }

PluginPort::PluginPort(LilvWorld *world, const LilvPlugin *rawPlugin,
                       const LilvPort *p, const float &argMinimum,
                       const float &argMaximum, const float &argDefault,
                       const uint32_t &argIndex)
    : _minimum{argMinimum}, _maximum{argMaximum}, _default{argDefault},
      _index{argIndex}, seq{nullptr} {
  if (lv2_AtomPort == nullptr) {
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

  if (lilv_port_is_a(rawPlugin, rawPort, lv2_InputPort)) {
    _direction = PluginPortDirection::input;
  } else if (lilv_port_is_a(rawPlugin, rawPort, lv2_OutputPort)) {
    _direction = PluginPortDirection::output;
  }

  if (lilv_port_is_a(rawPlugin, rawPort, lv2_AudioPort)) {
    _type = PluginPortType::audio;
  } else if (lilv_port_is_a(rawPlugin, rawPort, lv2_ControlPort)) {
    _type = PluginPortType::control;
  } else if (lilv_port_is_a(rawPlugin, rawPort, lv2_AtomPort)) {
    if (lilv_port_supports_event(rawPlugin, rawPort, lv2_MidiEvent)) {
      _type = PluginPortType::midi;
    }
  }

  const LilvNode *sym = lilv_port_get_symbol(rawPlugin, rawPort);
  _symbol = lilv_node_as_string(sym);
  sym = lilv_port_get_name(rawPlugin, rawPort);
  _name = lilv_node_as_string(sym);
}

void PluginPort::print() const {
  std::cout << "Port " << _name << '\n';
  std::cout << "\tIndex: " << _index << '\n';
  std::cout << "\tSymbol: " << _symbol << '\n';
  std::cout << "\tMinimum: " << _minimum << '\n';
  std::cout << "\tMaximum: " << _maximum << '\n';
  std::cout << "\tDefault: " << _default << '\n';
}

PluginPortType PluginPort::type() { return _type; }

void PluginPort::buffer(LilvInstance *instance, const audio::Buffer buf) {
  lilv_instance_connect_port(instance, _index, buf->data());
}

void PluginPort::buffer(LilvInstance *instance, const midi::Buffer buf) {
  seq = new LV2_Atom_Sequence;
  seq->atom.size = sizeof(LV2_Atom_Sequence_Body);
  seq->atom.type = map.map(map.handle, LV2_ATOM__Sequence);
  seq->body.unit = 0;
  seq->body.pad = 0;

  MIDINoteEvent ev;
  ev.event.time.frames = IO::playHead(); // sample position
  ev.event.body.type = map.map(map.handle, LV2_MIDI__MidiEvent);
  ev.event.body.size = 3;
  ev.msg = &(buf->type);
  lv2_atom_sequence_end(&seq->body, seq->atom.size);

  lilv_instance_connect_port(instance, _index, seq);
}

void PluginPort::buffer(LilvInstance *instance, const float &control) {
  lilv_instance_connect_port(instance, _index, (void *)&control);
}

PluginPortDirection PluginPort::direction() { return _direction; }
PluginPort::~PluginPort() {}
