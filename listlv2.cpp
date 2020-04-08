#include <iostream>
#include <lilv/lilv.h>


int main()
{
  LilvWorld* world = lilv_world_new();
  lilv_world_load_all(world);
  const LilvPlugins *list = lilv_world_get_all_plugins(world);
  LILV_FOREACH(plugins, i, list) {
    const LilvPlugin* p = lilv_plugins_get(list, i);
    LilvNode* n = lilv_plugin_get_name(p);
    std::string name = lilv_node_as_string(n);
    std::string url = lilv_node_as_uri(lilv_plugin_get_uri(p));
    std::cout << name << ' ' << url << '\n';
    lilv_node_free(n);
  }
  lilv_world_free(world);
  return 0;
}
