#include <iostream>
#include <map>

#include <maolan/audio/input.hpp>
#include <maolan/audio/io.hpp>
#include <maolan/audio/output.hpp>
#include <maolan/config.hpp>

using namespace maolan::audio;

std::vector<IO *> IO::_all;

bool IO::operator<(const IO &arg) { return (connected() < arg.connected()); }
void IO::sort() { std::sort(_all.begin(), _all.end()); }

IO::IO(const std::string &name, const bool &reg, const size_t &chs)
    : maolan::IO(name, reg) {
  _inputs.resize(chs);
  _outputs.resize(chs);
  for (int ch = 0; ch < chs; ++ch) {
    _inputs[ch] = new Input();
    _outputs[ch] = new Output();
  }
}

Buffer IO::pull(const std::size_t &channel) {
  if (channels() > channel) {
    auto &out = _outputs[channel];
    if (out == nullptr) {
      return nullptr;
    }
    return out->buffer();
  }
  std::cerr << _type << ' ' << _name << " has " << channels();
  std::cerr << " channels and channel " << channel + 1 << " requested!\n";
  return nullptr;
}

nlohmann::json IO::json() {
  auto result = maolan::IO::json();
  result["channels"] = channels();
  return result;
}

nlohmann::json IO::connections() {
  auto result = nlohmann::json::array();
  for (size_t i = 0; i < _inputs.size(); ++i) {
    auto &input = _inputs[i];
    auto injson = input->json(_name, i);
    if (injson != nullptr) {
      result.push_back(injson);
    }
  }
  if (result.size() == 0) {
    return nullptr;
  }
  return result;
}

size_t IO::connected() const {
  std::map<IO *, bool> tracks;
  size_t size = 0;

  for (auto const &input : _inputs) {
    tracks.clear();
    for (auto const &connection : input->connections()) {
      tracks[connection->to()] = true;
    }
    size += tracks.size();
  }
  return size;
}

void IO::connect(IO *to) {
  for (std::size_t channel = 0; channel < _inputs.size(); ++channel) {
    connect(to, channel, channel);
  }
}

void IO::connect(IO *to, size_t inch, size_t outch) {
  if (inch < _inputs.size()) {
    _inputs[inch]->connect(to, outch);
    to->backref(this, inch, outch);
  }
}

void IO::backref(IO *to, size_t inch, size_t outch) {
  if (outch < _outputs.size()) {
    _outputs[inch]->connect(to, outch);
  }
}

void IO::fetch() {
  for (auto &input : _inputs) {
    input->fetch();
  }
}

void IO::process() {
  for (auto &input : _inputs) {
    input->process();
  }
}

bool IO::ready() const {
  if (_processing) {
    return false;
  }
  if (_processed) {
    return true;
  }
  for (const auto &input : _inputs) {
    for (const auto &connection : input->connections()) {
      if (!connection->to()->ready()) {
        return false;
      }
    }
  }
  return true;
}

std::size_t IO::channels() const { return _outputs.size(); }
