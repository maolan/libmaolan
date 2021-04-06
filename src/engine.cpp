#include <algorithm>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "maolan/audio/clip.hpp"
#include "maolan/audio/connectable.hpp"
#include "maolan/audio/track.hpp"
#include "maolan/engine.hpp"
#include "maolan/io.hpp"
#include "maolan/midi/clip.hpp"
#include "maolan/midi/track.hpp"

#if OSS == ON
#include "maolan/audio/oss/in.hpp"
#include "maolan/audio/oss/out.hpp"
#endif

#if LV2 == ON
#include "maolan/plugin/lv2/plugin.hpp"
#endif

using namespace maolan;


std::vector<Worker *> Engine::_workers;
static std::vector<std::string> audioNames =
{
#if OSS == ON
  "AudioOSSOut",
#endif
  "AudioTrack"
};
static std::vector<std::string> midiNames =
{
#if OSS == ON
  "MidiOSSOut",
#endif
  "MidiTrack"
};


void Engine::init(const int &threads)
{
  if (threads != 0)
  {
    int maxWorkers = std::thread::hardware_concurrency();
    auto realWorkerNumber =
        threads == -1 || threads > maxWorkers ? maxWorkers : threads;
    _workers.resize(realWorkerNumber);
    for (std::size_t i = 0; i < _workers.size(); ++i)
    {
      _workers[i] = new Worker();
    }
  }
  for (auto io = IO::begin(); io != nullptr; io = io->next())
  {
    io->init();
  }
#if LV2 == ON
  plugin::lv2::Plugin::allocate();
#endif
}


void Engine::quit()
{
  IO::quit();
  _workers.clear();
#if LV2 == ON
  plugin::lv2::Plugin::destroyWorld();
#endif
}


nlohmann::json Engine::json()
{
  nlohmann::json data;
  data["io"] = nlohmann::json::array();
  data["connections"] = nlohmann::json::array();
  for (auto io = IO::begin(); io != nullptr; io = io->next())
  {
    data["io"].push_back(io->json());
  }
  auto ioconns = maolan::audio::Connectable::json();
  if (ioconns != nullptr) 
  { 
    for (auto &c : ioconns) 
    { 
      data["connections"].push_back(c); 
    }
  }
  return data;
}


void Engine::save()
{
  chdir(Config::root.data());
  midi::Clip::saveAll();
  auto data = json();
  std::ofstream session{"session.json"};
  session << data.dump(2);
}


nlohmann::json Engine::load()
{
  chdir(Config::root.data());
  std::ifstream session{"session.json"};
  auto result = nlohmann::json::parse(session);
  std::map<std::string, IO *> ios;
  for (const auto &io : result["io"])
  {
    if (io["type"] == "AudioTrack")
    {
      auto track = new maolan::audio::Track(io["name"], io["channels"]);
      ios[io["name"]] = track;
      for (const auto &clipio : io["clips"])
      {
        const std::string &name = clipio["name"];
        auto clip = new maolan::audio::Clip(name, track);
        ios[name] = clip;
      }
    }
    else if (io["type"] == "MIDITrack")
    {
      auto track = new maolan::midi::Track(io["name"], 1);
      ios[io["name"]] = track;
      for (const auto &clipio : io["clips"])
      {
        auto clip = new maolan::midi::Clip(clipio["name"], track);
        ios[clipio["name"]] = clip;
      }
    }
#if OSS == ON
    else if (io["type"] == "AudioOSSOut")
    {
      auto bits = io["bits"];
      if (bits == 32)
      {
        auto out = new maolan::audio::OSSOut<int32_t>(io["name"]);
        ios[io["name"]] = out;
      }
      else if (bits == 16)
      {
        auto out = new maolan::audio::OSSOut<int16_t>(io["name"]);
        ios[io["name"]] = out;
      }
      else if (bits == 8)
      {
        auto out = new maolan::audio::OSSOut<int8_t>(io["name"]);
        ios[io["name"]] = out;
      }
    }
    else if (io["type"] == "AudioOSSIn")
    {
      auto bits = io["bits"];
      if (bits == 32)
      {
        auto out = new maolan::audio::OSSIn<int32_t>(io["name"]);
        ios[io["name"]] = out;
      }
      else if (bits == 16)
      {
        auto out = new maolan::audio::OSSIn<int16_t>(io["name"]);
        ios[io["name"]] = out;
      }
      else if (bits == 8)
      {
        auto out = new maolan::audio::OSSIn<int8_t>(io["name"]);
        ios[io["name"]] = out;
      }
    }
#endif
  }
  for (const auto &c : result["connections"])
  {
    auto &fromio = ios[c["name"]];
    if (std::find(audioNames.begin(), audioNames.end(), fromio->type()) != audioNames.end())
    {
      auto &fromch = c["channel"];
      auto &tojson = c["to"];
      for (auto &iojson : tojson)
      {
        auto toio = (audio::IO *)ios[iojson["name"]];
        auto &toch = iojson["channel"];
        ((audio::Connectable *)fromio)->connect(toio, fromch, toch);
      }
    }
  }
  return result;
}


void Engine::play() { IO::play(); }
void Engine::stop() { IO::stop(); }
