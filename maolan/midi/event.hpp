#pragma once

namespace maolan::midi {
class Event {
public:
  static unsigned const NOTE_MASK = 0xF0;
  static unsigned const NOTE_ON = 0x90;
  static unsigned const NOTE_OFF = 0x80;
  static unsigned const CHANNEL_MASK = 0xF;
  static unsigned const CONTROLER_ON = 0xB0;
  static unsigned const META = 0xFF;
};
} // namespace maolan::midi
