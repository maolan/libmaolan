#include <lv2/lv2plug.in/ns/ext/atom/forge.h>

#include <maolan/plugin/lv2/sequence.hpp>

using namespace maolan::plugin::lv2;

class MIDINoteEvent {
public:
  LV2_Atom_Event event;
  uint8_t msg[3];
};

static inline LV2_Atom_Event *_lv2_append_event(LV2_Atom_Sequence *seq,
                                                uint32_t capacity,
                                                const LV2_Atom_Event *event) {
  const uint32_t total_size = (uint32_t)sizeof(*event) + event->body.size;
  if (capacity - seq->atom.size < total_size) {
    return nullptr;
  }
  LV2_Atom_Event *e = lv2_atom_sequence_end(&seq->body, seq->atom.size);
  memcpy(e, event, total_size);
  seq->atom.size += lv2_atom_pad_size(total_size);
  return e;
}

Sequence::Sequence() {
  this->buffer_size = 4096;
  this->buffer = new char[this->buffer_size];
  seq = (LV2_Atom_Sequence *)buffer;
  seq->atom.size = sizeof(LV2_Atom_Sequence_Body);
  // seq->atom.type = map.map(map.handle, LV2_ATOM__Sequence);
  seq->body.unit = 0;
  seq->body.pad = 0;
}

void Sequence::addMidiNote(std::uint64_t pos, std::uint8_t key,
                           std::int8_t velocity) {
  std::uint8_t note_on = 0x90;

  MIDINoteEvent ev;
  ev.event.time.frames = pos; // sample position
  // ev.event.body.type = map.map(map.handle, LV2_MIDI__MidiEvent);
  ev.event.body.size = sizeof(ev.msg);

  ev.msg[0] = note_on;
  ev.msg[1] = key;
  ev.msg[2] = velocity;

  LV2_Atom_Event *e = _lv2_append_event(seq, this->buffer_size, &ev.event);
  (void)e;
}

void Sequence::clear() { seq->atom.size = sizeof(LV2_Atom_Sequence_Body); }
void *Sequence::data() { return buffer; }
