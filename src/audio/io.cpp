#include <iostream>

#include <maolan/audio/input.hpp>
#include <maolan/audio/output.hpp>
#include <maolan/audio/io.hpp>
#include <maolan/config.hpp>

using namespace maolan::audio;

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
    return _outputs[channel]->buffer();
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

void IO::backref(IO *to, size_t inch, size_t outch) {}

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

bool IO::leaf() {
  for (const auto &input : _inputs) {
    if (!input->leaf()) {
      return false;
    }
  }
  return true;
}

std::size_t IO::channels() const { return _outputs.size(); }
