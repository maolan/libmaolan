#include <dlfcn.h>

#include <maolan/dynamichw.hpp>

using namespace maolan;

DynamicHW::DynamicHW(const std::string &lib) {
  void *handle;

  handle = dlopen(lib.data(), RTLD_LAZY);
  if (handle == nullptr) {
    throw std::invalid_argument{"Can not load " + lib};
  }
  audio.out = (audio::audio_t)dlsym(handle, "createAudioOut");
  audio.in = (audio::audio_t)dlsym(handle, "createAudioIn");
  audio.list = (audio::list_t)dlsym(handle, "listAudio");
  midi.out = (midi::midi_t)dlsym(handle, "createMidiOut");
  midi.in = (midi::midi_t)dlsym(handle, "createMidiIn");
  midi.list = (midi::list_t)dlsym(handle, "listMidi");
}
