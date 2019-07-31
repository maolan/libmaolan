#include <iostream>
#include <pugixml.hpp>

#include <maolan/audio/clip.h>
#include <maolan/audio/connection.h>
#include <maolan/audio/ossin.h>
#include <maolan/audio/ossout.h>
#include <maolan/audio/track.h>
#include <maolan/utils.h>

namespace maolan
{
IO *node2IO(pugi::xml_node *n)
{
  audio::IO *io = nullptr;

  std::string name = n->name();
  if (name == "track")
  {
    std::cout << "nova traka" << std::endl;
    io = new audio::Track(n->attribute("name").value());
    return io;
  }
  else if (name == "clip")
  {
    std::cout << "novi clip" << std::endl;
    io = new audio::Clip(
        n->attribute("start").as_uint(), n->attribute("end").as_uint(),
        n->attribute("offset").as_uint(), n->attribute("path").value());
    for (auto parent = IO::begin(); parent != nullptr; parent = parent->next())
    {
      if (parent->name() == n->parent().parent().attribute("name").value())
      {
        io->parrent(parent);
        break;
      }
    }
    return io;
  }
  else if (name == "input")
  {
    std::cout << "novi input" << std::endl;
    io = new audio::OSSIn(n->attribute("device").value(),
                          n->attribute("channels").as_int());
    return io;
  }
  else if (name == "output")
  {
    std::cout << "novi output" << std::endl;
    io = new audio::OSSOut(n->attribute("device").value(),
                           n->attribute("channels").as_int());
    return io;
  }
  else if (name == "connection")
  {
    std::cout << "novi connection" << std::endl;
    return io;
  }
  return nullptr;
}

pugi::xml_node loadXml(const char *path)
{
  pugi::xml_document doc;
  pugi::xml_parse_result result =
      doc.load_file(path, pugi::parse_default | pugi::parse_declaration);
  if (!result)
  {
    std::cerr << "Parse error: " << result.description();
    std::cerr << ", character pos= " << result.offset << std::endl;
  }
  pugi::xml_node root = doc.document_element();
  return root;
}

} // namespace maolan
