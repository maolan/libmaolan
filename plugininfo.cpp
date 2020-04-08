#include <vector>
#include <string>
#include <iostream>
#include <lilv/lilv.h>


class Plugin
{
  public:
    Plugin(const std::string &argUri)
      : _identifier{argUri}
    {
      if (world == nullptr)
      {
        world = lilv_world_new();
        lilv_world_load_all(world);
        plugins = (LilvPlugins *)lilv_world_get_all_plugins(world);
      }
      uri(lilv_new_uri(world, _identifier.data()));
      rawPlugin = (LilvPlugin *)lilv_plugins_get_by_uri(plugins, _uri);
      auto val = lilv_plugin_get_name(rawPlugin);
      if (val) { _name = lilv_node_as_string(val); }
      val = lilv_plugin_get_author_name(rawPlugin);
      if (val) { _author = lilv_node_as_string(val); }
      val = lilv_plugin_get_author_homepage(rawPlugin);
      if (val) { _authorHomepage = lilv_node_as_string(val); }
      val = lilv_plugin_get_author_email(rawPlugin);
      if (val) { _authorEmail = lilv_node_as_string(val); }
      if (_authorEmail.size() > 7)
      {
        _authorEmail = _authorEmail.substr(7, std::string::npos);
      }
    }

    ~Plugin()
    {
      lilv_node_free((LilvNode *)_uri);
    }

    static LilvWorld *world;
    static LilvPlugins *plugins;

    void uri(const LilvNode *argUri) { _uri = argUri; }
    const LilvNode * uri() const { return _uri; }
    const std::string identifier() const { return _identifier; }
    const std::string name() const { return _name; }
    const std::string author() const { return _author; }
    const std::string authorHomepage() const { return _authorHomepage; }
    const std::string authorEmail() const { return _authorEmail; }

    void print()
    {
      std::cout << _name << ": " << _identifier << std::endl;
      std::cout << "Author: " << _author << " <" << _authorEmail << ">, " << _authorHomepage << std::endl;
    }

  protected:
    const LilvNode *_uri;
    const std::string _identifier;
    std::string _name;
    std::string _author;
    std::string _authorHomepage;
    std::string _authorEmail;
    LilvPlugin *rawPlugin;
};

LilvWorld *Plugin::world = nullptr;
LilvPlugins *Plugin::plugins = nullptr;


int main()
{
  Plugin p("http://calf.sourceforge.net/plugins/MultiChorus");
  p.print();
  lilv_world_free(Plugin::world);
  return 0;
}
