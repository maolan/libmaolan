#include <iostream>
#include <pugixml.hpp>

int main()
{
pugi::xml_document doc;
auto declarationNode = doc.append_child(pugi::node_declaration);
pugi::xml_node track= doc.append_child("track");
track.append_attribute("name") = "ime_trake";
pugi::xml_node clips = track.append_child("clips");
pugi::xml_node clip = clips.append_child("clip");
clip.append_attribute("name") = "ime_clipa";
clip.append_attribute("file") = "data/session.wav";

bool saveSucceeded = doc.save_file("test.xml");
assert(saveSucceeded);
return 0;
}

