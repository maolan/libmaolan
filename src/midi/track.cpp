#include <maolan/midi/clip.hpp>
#include <maolan/midi/input.hpp>
#include <maolan/midi/track.hpp>

using namespace maolan::midi;

std::vector<Track *> Track::_all;

Track::Track(const std::string &name, const std::size_t &channel)
    : IO{name, true}, _muted{false}, _armed{false}, _soloed{false},
      _channel{channel}, _first{nullptr}, _current{nullptr}, _last{nullptr} {
  _type = "MIDITrack";
  _input = new Input();
  _all.push_back(this);
}

void Track::fetch() {
  if (_current != nullptr) {
    _current->fetch();
  }
}

void Track::process() {
  if (_current == nullptr) {
    _output = nullptr;
    return;
  }
  if (_armed) {
    _output = _input->pull();
    // recording->write(buffer);
  } else if (!_muted && _playHead >= _current->startSample()) {
    _current->process();
    _output = _current->pull();
    for (auto buffer = _output; buffer != nullptr; buffer = buffer->next) {
      buffer->channel = _channel;
    }
  }
}

void Track::setup() {
  if (_armed && _recording == nullptr) {
    _recording = new midi::Clip("/tmp/clip.mid", this);
    _recording->startSample(_playHead);
    _recording->endSample(_playHead + Config::audioBufferSize);
    _current = _recording;
    _first = _current;
    _last = _current;
    _current->setup();
  } else if (_first == nullptr) {
    _current = nullptr;
  } else if (_playHead < _first->startSample()) {
    _current = nullptr;
  } else if (!_armed && _playHead > _last->endSample()) {
    _current = nullptr;
  } else {
    for (auto clip = _last; clip != nullptr; clip = clip->previous()) {
      if (clip->startSample() <= _playHead && _playHead < clip->endSample()) {
        _current = clip;
        break;
      }
    }
    _current->setup();
  }
}

void Track::add(Clip *clip) {
  if (_first == nullptr) {
    _first = clip;
    _last = clip;
  } else {
    for (auto cl = _first; cl != nullptr; cl = cl->next()) {
      if (clip->start() < cl->start()) {
        clip->next(cl);
        clip->previous(cl->previous());
        cl->previous()->next(clip);
        cl->previous(clip);
        if (clip->previous() == nullptr) {
          _first = clip;
        }
        if (clip->next() == nullptr) {
          _last->next(clip);
        }
        break;
      }
    }
  }
}

void Track::remove(Clip *clip) {
  if (_first == nullptr) {
    return;
  }
  for (auto cl = _first; cl != nullptr; cl = cl->next()) {
    if (cl == clip) {
      if (cl->next() != nullptr) {
        cl->next()->previous(cl->previous());
      }
      if (cl->previous() != nullptr) {
        cl->previous()->next(cl->next());
      }
      if (cl == _current) {
        _current = nullptr;
      }
      if (cl == _recording) {
        _recording = nullptr;
      }
      if (cl == _first) {
        _first = cl->next();
      }
      if (cl == _last) {
        _last = cl->previous();
      }
      cl->parent(nullptr);
      return;
    }
  }
}

Buffer Track::pull() {
  if (_muted) {
    return nullptr;
  }
  return _output;
}

nlohmann::json Track::json() {
  auto data = IO::json();
  data["clips"] = nlohmann::json::array();
  for (auto clip = clips(); clip != nullptr; clip = clip->next()) {
    data["clips"].push_back(clip->json());
  }
  return data;
}

Clip *Track::clips() { return _first; }
void Track::mute(const bool &value) { _muted = value; }
void Track::arm(const bool &value) { _armed = value; }
void Track::solo(const bool &value) { _soloed = value; }
std::vector<Track *> Track::all() { return _all; }
Track::~Track() { (void)std::remove(_all.begin(), _all.end(), this); }
