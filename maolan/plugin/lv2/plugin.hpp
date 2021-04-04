#pragma once
#include <lilv/lilv.h>
#include <string>
#include <vector>

#include "maolan/audio/buffer.hpp"
#include "maolan/frame.hpp"
#include "maolan/plugin/io.hpp"
#include "maolan/plugin/lv2/info.hpp"
#include "maolan/plugin/lv2/port.hpp"


namespace maolan::plugin::lv2
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


class Plugin : public plugin::IO
{
public:
  Plugin(const std::string &argUri);
  ~Plugin();

  static void allocate();
  static void destroyWorld();
  static const void *portValue(const char *port_symbol, void *user_data,
                               uint32_t *size, uint32_t *type);

  virtual void fetch();
  virtual void process();
  virtual std::size_t ports(const std::string &type = "audio",
                            const std::string &direction = "out");

  void uri(const LilvNode *argUri);
  const LilvNode *uri() const;
  const std::string identifier() const;
  const std::string name() const;
  const Author author() const;
  void print() const;
  const PluginInfo info() const;
  void saveSession() const;

protected:
  static LilvWorld *world;
  static LilvPlugins *plugins;
  static audio::Buffer emptyBuffer;

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
} // namespace maolan::plugin::lv2
