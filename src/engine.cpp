#include <iostream>
#include <thread>

#include "maolan/audio/clip.hpp"
#include "maolan/audio/connectable.hpp"
#include "maolan/audio/oss/in.hpp"
#include "maolan/audio/oss/out.hpp"
#include "maolan/audio/track.hpp"
#include "maolan/engine.hpp"
#include "maolan/io.hpp"
#include "maolan/midi/clip.hpp"
#include "maolan/midi/track.hpp"
#include "maolan/plugin/lv2/plugin.hpp"


using namespace maolan;


std::vector<Worker *> Engine::_workers;


void Engine::init(const int &threads)
{
  int maxWorkers = std::thread::hardware_concurrency();
  auto realWorkerNumber =
      threads == -1 || threads > maxWorkers ? maxWorkers : threads;
  _workers.resize(realWorkerNumber);
  for (std::size_t i = 0; i < _workers.size(); ++i)
  {
    _workers[i] = new Worker();
  }
  for (auto io = IO::begin(); io != nullptr; io = io->next())
  {
    io->init();
  }
  plugin::lv2::Plugin::allocate();
}


void Engine::quit()
{
  IO::quit();
  _workers.clear();
  plugin::lv2::Plugin::destroyWorld();
}


nlohmann::json Engine::json()
{
  nlohmann::json data;
  data["io"] = nlohmann::json::array();
  for (auto io = IO::begin(); io != nullptr; io = io->next())
  {
    data["io"].push_back(io->json());
  }
  auto c = maolan::audio::Connectable::json();
  if (c != nullptr) { data["connections"] = c; }
  else { data["connections"] = nlohmann::json::array(); }
  return data;
}


void Engine::save()
{
  midi::Clip::saveAll();
  auto data = json();
  std::cout << data.dump(2) << '\n';
}


nlohmann::json Engine::load(const std::string &path)
{
  std::ifstream session{path + "/session.json"};
  auto result = nlohmann::json::parse(session);
  for (const auto &io : result["io"])
  {
    if (io["type"] == "AudioTrack")
    {
      auto track = new maolan::audio::Track(io["name"], io["channels"]);
      for (const auto &clipio : io["clips"])
      {
        const std::string name = clipio["name"];
        auto clip = new maolan::audio::Clip(path + "/" + name, track);
      }
    }
    else if (io["type"] == "MIDITrack")
    {
      auto track = new maolan::midi::Track(io["name"], 1);
      for (const auto &clipio : io["clips"])
      {
        auto clip = new maolan::midi::Clip(clipio["name"], track);
      }
    }
    else if (io["type"] == "AudioOSSOut")
    {
      auto bits = io["bits"];
      if (bits == 32)
      {
        auto out = new maolan::audio::OSSOut<int32_t>(io["name"]);
      }
      else if (bits == 16)
      {
        auto out = new maolan::audio::OSSOut<int16_t>(io["name"]);
      }
      else if (bits == 8)
      {
        auto out = new maolan::audio::OSSOut<int8_t>(io["name"]);
      }
    }
    else if (io["type"] == "AudioOSSIn")
    {
      auto bits = io["bits"];
      if (bits == 32)
      {
        auto out = new maolan::audio::OSSIn<int32_t>(io["name"]);
      }
      else if (bits == 16)
      {
        auto out = new maolan::audio::OSSIn<int16_t>(io["name"]);
      }
      else if (bits == 8)
      {
        auto out = new maolan::audio::OSSIn<int8_t>(io["name"]);
      }
    }
  }
  return result;
}


void Engine::play() { IO::play(); }
void Engine::stop() { IO::stop(); }
