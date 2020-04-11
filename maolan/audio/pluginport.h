#pragma once
#include <string>
#include <lilv/lilv.h>
#include <maolan/audio/buffer.h>


namespace maolan::audio
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
    PluginPort(
      LilvWorld *world,
      const LilvPlugin *rawPlugin,
      const LilvPort *p,
      const float &minimum,
      const float &maximum,
      const float &defaultValue,
      const uint32_t &index
    );
    ~PluginPort();

    PluginPortDirection direction() { return _direction; }
    PluginPortType type() { return _type; }
    void print() const;
    void buffer(LilvInstance *instance, const Buffer buf);
    Buffer buffer(LilvInstance *instance);

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
};
} // namespace maolan::audio
