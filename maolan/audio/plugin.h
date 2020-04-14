#pragma once
#include <vector>
#include <string>
#include <lilv/lilv.h>
#include <maolan/frame.h>
#include <maolan/audio/buffer.h>
#include <maolan/audio/plugininfo.h>
#include <maolan/audio/pluginport.h>


namespace maolan::audio
{
class Author
{
  public:
    std::string name;
    std::string homepage;
    std::string email;
};


class Ports
{
  public:
    std::vector<PluginPort *> audio;
    std::vector<PluginPort *> control;
    std::vector<PluginPort *> midi;
};


class Plugin
{
  public:
    Plugin(const std::string &argUri);
    ~Plugin();

    static void init();
    static void destroyWorld();
    static const void * portValue(const char *port_symbol, void *user_data, uint32_t *size, uint32_t *type);

    void uri(const LilvNode *argUri);
    const LilvNode * uri() const;
    const std::string identifier() const;
    const std::string name() const;
    const Author author() const;
    void print() const;
    const Frame * const process(const Frame * const inputs);
    const PluginInfo info() const;
    void saveSession() const;

  protected:
    static LilvWorld *world;
    static LilvPlugins *plugins;
    static Buffer emptyBuffer;

    const LilvNode *_uri;
    const LilvPlugin *rawPlugin;
    LilvInstance *instance;
    const std::string _identifier;
    std::string _name;
    Author _author;
    Ports input;
    Ports output;
    LV2_URID_Map _lv2_urid_map;
};
} // namespace maolan::audio
