#include <algorithm>
#include <random>
#include <string>

#include <maolan/audio/clip.hpp>
#include <maolan/audio/input.hpp>
#include <maolan/audio/io.hpp>
#include <maolan/audio/output.hpp>
#include <maolan/audio/track.hpp>
#include <maolan/config.hpp>

using namespace maolan::audio;

std::vector<Track *> Track::_all;

std::string random_string(const size_t &size) {
  std::string str("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
  std::random_device rd;
  std::mt19937 generator(rd());
  std::shuffle(str.begin(), str.end(), generator);
  return str.substr(0, size);
}

Track::Track(const std::string &name, const std::size_t &ch)
    : IO{name, true, ch}, _muted{false}, _armed{false}, _soloed{false},
      _volume{1.0}, _first{nullptr}, _current{nullptr}, _last{nullptr},
      _recording{nullptr} {
  _type = "AudioTrack";
  _all.push_back(this);
}

void Track::fetch() {
  IO::fetch();
  if (_current != nullptr) {
    _current->fetch();
  }
}

void Track::process() {
  IO::process();
  if (_current != nullptr) {
    _current->process();
  }
  auto const chs = channels();
  auto frame = new Frame(chs, 0, 0);
  if (_armed) {
    for (std::size_t channel = 0; channel < chs; ++channel) {
      frame->audio[channel] = _inputs[channel]->pull();
    }
    _recording->write(frame);
    if (!_muted) {
      for (int i = 0; i < frame->audio.size(); ++i) {
        _outputs[i]->buffer(frame->audio[i]);
      }
    }
    delete frame;
  } else if (!_muted && _current != nullptr && _playHead >= _current->start()) {
    for (std::size_t channel = 0; channel < chs; ++channel) {
      frame->audio[channel] = _current->pull(channel);
    }
    auto &result = frame;
    // TODO: process plugins
    for (int i = 0; i < result->audio.size(); ++i) {
      _outputs[i]->buffer(result->audio[i]);
    }
    delete result;
  }
}

void Track::setup() {
  if (_armed && _recording == nullptr) {
    _recording = new Clip(random_string(8), this);
    _recording->start(_playHead);
    _recording->end(_playHead + Config::audioBufferSize);
    _current = _recording;
    _first = _current;
    _last = _current;
    _current->setup();
  } else if (_first == nullptr) {
    _current = nullptr;
  } else if (_playHead < _first->start()) {
    _current = nullptr;
  } else if (!_armed && _playHead > _last->end()) {
    _current = nullptr;
  } else {
    for (auto clip = _last; clip != nullptr; clip = clip->previous()) {
      if (clip->start() <= _playHead && clip->end() > _playHead) {
        _current = clip;
        _current->setup();
        break;
      }
    }
  }
}

void Track::add(Clip *clip) {
  clip->next(nullptr);
  clip->previous(nullptr);
  if (_first == nullptr) {
    _first = clip;
    _last = clip;
  } else if (_first->start() > clip->start()) {
    _first->previous(clip);
    clip->next(_first);
    _first = clip;
  } else if (_last->start() < clip->start()) {
    _last->next(clip);
    clip->previous(_last);
    _last = clip;
  } else {
    for (auto cl = _last; cl != nullptr; cl = cl->previous()) {
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

#ifdef LV2_ENABLED
void Track::remove(plugin::lv2::Plugin *plugin) {
  for (std::size_t i = 0; i < _plugins.size(); ++i) {
    if (_plugins[i] == plugin) {
      _plugins.erase(_plugins.begin() + i);
      return;
    }
  }
}

void Track::add(plugin::lv2::Plugin *plugin) { _plugins.push_back(plugin); }
#endif

Buffer Track::pull(const std::size_t &channel) {
  if (_muted) {
    return nullptr;
  }
  auto b = IO::pull(channel);
  if (_volume == 1.0) {
    return b;
  }
  auto result = std::make_shared<BufferData>(Config::audioBufferSize);
  for (size_t i = 0; i < Config::audioBufferSize; ++i) {
    result->data()[i] = b->data()[i] * _volume;
  }
  return result;
}

nlohmann::json Track::json() {
  auto data = audio::IO::json();
  data["clips"] = nlohmann::json::array();
  for (auto clip = _first; clip != nullptr; clip = clip->next()) {
    data["clips"].push_back(clip->json());
  }
  return data;
}

void Track::init() {
  for (auto clip = _first; clip != nullptr; clip = clip->next()) {
    clip->init();
  }
}

void Track::volume(const float &value) {
  if (value < 0.0) {
    _volume = 0.0;
  } else if (value > 1.0) {
    _volume = 1.0;
  } else {
    // TODO: make it logarithmic
    _volume = value;
  }
}

bool Track::mute() { return _muted; }
bool Track::arm() { return _armed; }
bool Track::solo() { return _soloed; }
float Track::volume() { return _volume; }
void Track::mute(const bool &value) { _muted = value; }
void Track::arm(const bool &value) { _armed = value; }
void Track::solo(const bool &value) { _soloed = value; }
Clip *Track::clips() { return _first; }
std::vector<Track *> Track::all() { return _all; }
Track::~Track() { (void)std::remove(_all.begin(), _all.end(), this); }
