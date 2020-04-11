#include <maolan/audio/io.h>
#include <maolan/io.h>
#include <pugixml.hpp>

namespace maolan
{
IO *xmlElement2IO(pugi::xml_node *n);
pugi::xml_node loadXml(const char *path = "data/example.xml");
} // namespace maolan
