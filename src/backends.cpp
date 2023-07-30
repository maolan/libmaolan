#include <maolan/backends.hpp>

using namespace maolan;

static std::vector<std::string> backendNames = {"oss", "alsa", "sndio"};
std::map<std::string, DynamicHW *> Backends::_backends;

void Backends::load() {
  std::string libPrefix;
  std::filesystem::path libPath;

  libPrefix = PREFIX;
  libPrefix += "/lib/libmaolan";
  _backends.clear();
  for (const auto &backend : backendNames) {
    libPath = libPrefix + backend + ".so";
    if (exists(libPath)) {
      _backends[backend] = new DynamicHW(libPath);
    }
  }
}

DynamicHW *Backends::find(const std::string &backend) {
  auto found = _backends.find(backend);
  if (found == _backends.end()) {
    return nullptr;
  }
  return found->second;
}
