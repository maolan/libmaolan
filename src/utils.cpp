#include <iostream>
#include <pugixml.hpp>

#include "maolan/audio/clip.hpp"
#include "maolan/audio/connection.hpp"
#include "maolan/audio/oss/in.hpp"
#include "maolan/audio/oss/out.hpp"
#include "maolan/audio/track.hpp"
#include "maolan/io.hpp"
#include "maolan/utils.hpp"

namespace maolan
{
IO *xmlElement2IO(pugi::xml_node *n)
{
  audio::IO *io = nullptr;

  // std::string name = n->name();
  // if (name == "track")
  // {
    // io = new audio::Track(n->attribute("name").value(), 1);
    // return io;
  // }
  // else if (name == "clip")
  // {
    // io = new audio::Clip(
      // n->attribute("path").value(),
      // n->attribute("start").as_uint(),
      // n->attribute("end").as_uint(),
      // n->attribute("offset").as_uint()
    // );
    // for (auto parent = IO::begin(); parent != nullptr; parent = parent->next())
    // {
      // if (parent->name() == n->parent().parent().attribute("name").value())
      // {
        // io->parent(parent);
        // break;
      // }
    // }
    // return io;
  // }
  // else if (name == "input")
  // {
    // io = new audio::OSSIn(n->attribute("device").value(),
                          // n->attribute("channels").as_int());
    // return io;
  // }
  // else if (name == "output")
  // {
    // io = new audio::OSSOut(n->attribute("device").value(),
                           // n->attribute("channels").as_int());
    // return io;
  // }
  // else if (name == "connection")
  // {
    // maolan::audio::OSSOut *from = nullptr;
    // maolan::audio::Track *to = nullptr;
    // for (auto item = IO::begin(); item != nullptr; item = item->next())
    // {
      // if (item->type() == n->attribute("from").value())
      // {
        // from = (maolan::audio::OSSOut *)item;
      // }
      // if (item->name() == n->attribute("to").value())
      // {
        // to = dynamic_cast<maolan::audio::Track *>(item);
      // }
    // }
    // from->connect(to);
  // }
  return io;
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
