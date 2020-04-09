#include <maolan/audio/plugin.h>


int main()
{
  maolan::audio::Plugin p("http://drumgizmo.org/lv2");
  p.print();
  maolan::audio::Plugin::destroyWorld();
  return 0;
}
