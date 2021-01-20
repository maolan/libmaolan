#pragma once
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <memory>


namespace maolan::plugin::lv2
{
class Sequence
{
public:
  Sequence();
  void clear();
  void addMidiNote(std::uint64_t pos, std::uint8_t key, std::int8_t velocity);
  void *data();

private:
  void *buffer;
  std::size_t buffer_size;
  LV2_Atom_Sequence *seq;
};
} // namespace maolan::plugin::lv2
