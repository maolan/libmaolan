#include <iostream>
#include <maolan/audio/io.hpp>
#include <maolan/io.hpp>

using namespace maolan;

bool IO::_rec{false};
bool IO::_playing{false};
bool IO::_quit{false};
std::atomic_size_t IO::_playHead{0};
std::atomic_size_t IO::_active{0};
std::atomic_bool IO::_all_processed{false};
std::mutex IO::_m;
std::condition_variable IO::_cv;
io_t IO::_all;
size_t ioindex{0};

IO::IO(const std::string &name, const bool &reg) : _name{name}, _data{nullptr} {
  if (reg) {
    _all.push_back(this);
  }
}

IO::~IO() {
  _all.erase(std::remove(_all.begin(), _all.end(), this), _all.end());
}

void IO::work() {
  ++_active;
  setup();
  fetch();
  process();
  processed(true);
  processing(false);
  --_active;
}

IO *IO::task() {
  std::unique_lock<std::mutex> lk(_m);
  _cv.wait(lk, check);
  if (_quit) {
    return nullptr;
  }
  auto &result = _all[ioindex];
  result->processing(true);
  lk.unlock();
  if (!_quit) {
    _cv.notify_one();
  }
  return result;
}

bool IO::check() {
  if (_quit) {
    return true;
  }
  if (!_playing) {
    return false;
  }
  if (_all.size() == 0) {
    return false;
  }
  bool more_work = false;
  for (ioindex = 0; ioindex < _all.size(); ++ioindex) {
    auto &io = _all[ioindex];
    more_work |= !io->processed();
    if (io->ready() && !io->processed()) {
      return true;
    }
  }
  if (!more_work) {
    _all_processed = true;
    _all_processed.notify_one();
  }
  return false;
}

void IO::tick() {
  _playHead += Config::audioBufferSize;
  if (Config::tempoIndex + 1 < Config::tempos.size()) {
    auto tempo = Config::tempos[Config::tempoIndex];
    while (Config::tempoIndex < Config::tempos.size() &&
           _playHead <= tempo.time) {
      ++Config::tempoIndex;
      tempo = Config::tempos[Config::tempoIndex];
    }
  }
  for (const auto &io : IO::all()) {
    io->processed(false);
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
  _all_processed = true;
  _all_processed.notify_one();
  _cv.notify_all();
}

bool IO::allready() {
  for (const auto &io : _all) {
    if (!io->ready()) {
      return false;
    }
  }
  return true;
}

void IO::drain() {
  if (_all_processed) {
    return;
  }
  _all_processed.wait(true);
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
  audio::IO::sort();
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
const io_t IO::all() { return _all; }
bool IO::playing() { return _playing; }
bool IO::quitting() { return _quit; }
bool IO::processed() const { return _processed; }
void IO::processed(const bool &p) { _processed = p; }
bool IO::ready() const { return _processed && !_processing; }
bool IO::processing() const { return _processing; }
void IO::processing(const bool &p) { _processing = p; }
