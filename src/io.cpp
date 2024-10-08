#include <maolan/io.hpp>

using namespace maolan;

bool IO::_rec = false;
bool IO::_playing = false;
bool IO::_quit = false;
std::atomic_size_t IO::_playHead = 0;
std::atomic_size_t IO::_index{0};
std::atomic_size_t IO::_line{0};
std::mutex IO::_m;
std::condition_variable IO::_cv;
ios_t IO::_all;
std::vector<ios_t> IO::_ordered;

IO::IO(const std::string &name, const bool &reg) : _name{name}, _data{nullptr} {
  if (reg) {
    _all.push_back(this);
  }
}

IO::~IO() {
  _all.erase(std::remove(_all.begin(), _all.end(), this), _all.end());
}

void IO::work() {
  setup();
  fetch();
  process();
}

IO *IO::task() {
  std::unique_lock<std::mutex> lk(_m);
  _cv.wait(lk, check);
  if (_quit) {
    return nullptr;
  }
  auto &line = _ordered[_line];
  auto &result = line[_index];
  ++_index;
  lk.unlock();
  _cv.notify_one();
  return result;
}

bool IO::check() {
  if (_quit) {
    return true;
  }
  if (!_playing) {
    return false;
  }
  if (_ordered.size() == 0) {
    return false;
  }
  if (_line >= _ordered.size()) {
    return false;
  }
  auto &line = _ordered[_line];
  if (_index >= line.size()) {
    ++_line;
    _index = 0;
    if (_line >= _ordered.size()) {
      return false;
    }
  }
  return true;
}

void IO::tick() {
  _index = 0;
  _line = 0;
  _playHead += Config::audioBufferSize;
  if (Config::tempoIndex + 1 < Config::tempos.size()) {
    auto tempo = Config::tempos[Config::tempoIndex];
    while (Config::tempoIndex < Config::tempos.size() &&
           _playHead <= tempo.time) {
      ++Config::tempoIndex;
      tempo = Config::tempos[Config::tempoIndex];
    }
  }
  _cv.notify_all();
}

void IO::play() {
  _playHead -= Config::audioBufferSize;
  _playing = true;
  _cv.notify_all();
}

void IO::stop() {
  _playing = false;
  _cv.notify_all();
}

void IO::quit() {
  _quit = true;
  _cv.notify_all();
}

nlohmann::json IO::json() {
  nlohmann::json data;
  data["name"] = _name;
  data["type"] = _type;
  return data;
}

bool IO::exists(std::string_view n) {
  for (const auto &io : _all) {
    if (io->name() == n) {
      return true;
    }
  }
  return false;
}

void IO::initall() {
  for (const auto &io : _all) {
    io->init();
  }
}

IO *IO::find(const std::string &name) {
  for (const auto &io : _all) {
    if (io->name() == name) {
      return io;
    }
  }
  return nullptr;
}

void IO::reorder() {
  bool done;

  _ordered.clear();
  do {
    ios_t line;

    done = true;
    for (const auto &io : _all) {
      if (ordered(io)) {
        continue;
      }
      if (io->leaf()) {
        done = false;
        line.push_back(io);
      }
    }
    if (!done) {
      _ordered.push_back(line);
    }
  } while (!done);
}

bool IO::ordered(IO *target) {
  for (const auto &line : _ordered) {
    for (const auto &io : line) {
      if (io == target) {
        return true;
      }
    }
  }
  return false;
}

void IO::parent(IO *) {}
void IO::rec(bool record) { _rec = record; }
bool IO::rec() { return _rec; }
std::string IO::type() { return _type; }
void IO::type(const std::string &argType) { _type = argType; }
std::string IO::name() { return _name; }
void IO::name(const std::string &argName) { _name = argName; }
uint64_t IO::playHead() { return _playHead; }
void IO::playHead(const std::size_t &argPlayHead) { _playHead = argPlayHead; }
void *IO::data() { return _data; }
void IO::data(void *d) { _data = d; }
void IO::init() {}
nlohmann::json IO::connections() { return nullptr; }
void IO::setup() {}
void IO::fetch() {}
void IO::process() {}
void IO::readhw() {}
void IO::writehw() {}
const ios_t IO::all() { return _all; }
const std::vector<ios_t> IO::ordered() { return _ordered; }
bool IO::playing() { return _playing; }
bool IO::quitting() { return _quit; }
bool IO::leaf() { return false; }
