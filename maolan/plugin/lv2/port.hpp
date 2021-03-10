#pragma once
#include <lilv/lilv.h>
#include <string>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include "maolan/audio/buffer.hpp"
#include "maolan/midi/buffer.hpp"


namespace maolan::plugin::lv2
{
enum class PluginPortDirection
{
  input,
  output
};


enum class PluginPortType
{
  audio,
  control,
  midi,
};


class PluginPort
{
public:
  PluginPort(LilvWorld *world, const LilvPlugin *rawPlugin, const LilvPort *p,
             const float &minimum, const float &maximum,
             const float &defaultValue, const uint32_t &index);
  ~PluginPort();

  PluginPortDirection direction();
  PluginPortType type();
  void print() const;
  void buffer(LilvInstance *instance, const audio::Buffer buf);
  void buffer(LilvInstance *instance, const midi::Buffer buf);
  void buffer(LilvInstance *instance, const float &control);

protected:
  static LilvNode *lv2_AtomPort;
  static LilvNode *lv2_AudioPort;
  static LilvNode *lv2_ControlPort;
  static LilvNode *lv2_CVPort;
  static LilvNode *lv2_EventPort;
  static LilvNode *lv2_InputPort;
  static LilvNode *lv2_MidiEvent;
  static LilvNode *lv2_OutputPort;
  static LilvNode *lv2_ConnectionOptional;
  static LV2_URID_Map map;

  float _minimum;
  float _maximum;
  float _default;
  PluginPortDirection _direction;
  PluginPortType _type;
  bool _optional;
  std::string _symbol;
  std::string _name;
  LilvPort *rawPort;
  uint32_t _index;
  LV2_Atom_Sequence *seq = nullptr;
};
} // namespace maolan::plugin::lv2
