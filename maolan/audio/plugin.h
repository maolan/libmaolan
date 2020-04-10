#pragma once
#include <vector>
#include <string>
#include <lilv/lilv.h>
#include <maolan/audio/buffer.h>


namespace maolan::audio
{
class Author
{
  public:
    std::string name;
    std::string homepage;
    std::string email;
};


class PluginPort;
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
    Buffer process(Buffer in_buf);

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
