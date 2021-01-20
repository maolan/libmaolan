#pragma once
#include <pugixml.hpp>
#include "maolan/audio/io.hpp"
#include "maolan/io.hpp"


namespace maolan
{
IO *xmlElement2IO(pugi::xml_node *n);
pugi::xml_node loadXml(const char *path = "data/example.xml");
} // namespace maolan
