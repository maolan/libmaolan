#include <iostream>
#include <pugixml.hpp>

int main()
{
  pugi::xml_document doc;
  auto declarationNode = doc.append_child(pugi::node_declaration);
  pugi::xml_node tracks= doc.append_child("tracks");
    pugi::xml_node track= tracks.append_child("track");
      track.append_attribute("name") = "prva_traka";
        pugi::xml_node clips = track.append_child("clips");
          pugi::xml_node clip1 = clips.append_child("clip");
            clip1.append_attribute("name") = "first_clip";
            clip1.append_attribute("file") = "data/session.wav";
            clip1.append_attribute("start") = "0";
            clip1.append_attribute("end") = "630000";
            clip1.append_attribute("offset") = "0";
          pugi::xml_node clip2 = clips.append_child("clip");
            clip2.append_attribute("name") = "second_clip";
            clip2.append_attribute("file") = "data/session.wav";
            clip2.append_attribute("start") = "630000";
            clip2.append_attribute("end") = "1260000";
            clip2.append_attribute("offset") = "0";
        pugi::xml_node connections = track.append_child("connections");
          pugi::xml_node connection1 = connections.append_child("connection");
            connection1.append_attribute("name") = "first_connection";
        pugi::xml_node inputs = track.append_child("inputs");
          pugi::xml_node input1= inputs.append_child("input");
            input1.append_attribute("device") = "/dev/dsp";
            input1.append_attribute("channels") = "2";
        pugi::xml_node outputs = track.append_child("outputs");
          pugi::xml_node output1= outputs.append_child("output");
            output1.append_attribute("device") = "/dev/dsp";
            output1.append_attribute("channels") = "2";
  bool saveSucceeded = doc.save_file("example.xml");
  assert(saveSucceeded);
  return 0;
}
