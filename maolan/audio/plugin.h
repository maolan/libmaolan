#pragma once
#include <vector>
#include <string>
#include <lilv/lilv.h>
#include <maolan/audio/buffer.h>
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

    static void destroyWorld();

    void uri(const LilvNode *argUri);
    const LilvNode * uri() const;
    const std::string identifier() const;
    const std::string name() const;
    const Author author() const;
    void print() const;
    std::vector<Buffer> process(const std::vector<Buffer> &in_buf);

  protected:
    static LilvWorld *world;
    static LilvPlugins *plugins;

    const LilvNode *_uri;
    const LilvPlugin *rawPlugin;
    LilvInstance *instance;
    const std::string _identifier;
    std::string _name;
    Author _author;
    Ports input;
    Ports output;
};
} // namespace maolan::audio
