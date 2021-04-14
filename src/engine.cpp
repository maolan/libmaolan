#include <algorithm>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "maolan/audio/clip.hpp"
#include "maolan/audio/track.hpp"
#include "maolan/engine.hpp"
#include "maolan/io.hpp"
#include "maolan/midi/clip.hpp"
#include "maolan/midi/track.hpp"

#ifdef OSS_ENABLED
#include "maolan/audio/oss/in.hpp"
#include "maolan/audio/oss/out.hpp"
#endif

#ifdef LV2_ENABLED
#include "maolan/plugin/lv2/plugin.hpp"
#endif


using namespace maolan;


std::vector<Worker *> Engine::_workers;
static std::vector<std::string> audioNames = {
#ifdef OSS_ENABLED
    "AudioOSSOut",
#endif
    "AudioTrack"};
static std::vector<std::string> midiNames = {
#ifdef OSS_ENABLED
    "MidiOSSOut",
#endif
    "MidiTrack"};


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
#ifdef LV2_ENABLED
  plugin::lv2::Plugin::allocate();
#endif
}


void Engine::quit()
{
  IO::quit();
  _workers.clear();
#ifdef LV2_ENABLED
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
    auto connections = io->connections();
    if (connections != nullptr)
    {
      for (auto &connection : connections)
      {
        data["connections"].push_back(connection);
      }
    }
  }
  return data;
}


void Engine::save()
{
  midi::Clip::saveAll();
  auto data = Engine::json();
  // std::ofstream session{"session.json"};
  // session << data.dump(2) << '\n';
  std::cout << data.dump(2) << '\n';
}


nlohmann::json Engine::load()
{
  chdir(Config::root.data());
  std::ifstream session{"session.json"};
  auto result = nlohmann::json::parse(session);
  for (const auto &io : result["io"])
  {
    if (io["type"] == "AudioTrack")
    {
      auto track = new maolan::audio::Track(io["name"], io["channels"]);
      for (const auto &clipio : io["clips"])
      {
        new maolan::audio::Clip(clipio["name"], track);
      }
    }
    else if (io["type"] == "MIDITrack")
    {
      auto track = new maolan::midi::Track(io["name"], 1);
      for (const auto &clipio : io["clips"])
      {
        new maolan::midi::Clip(clipio["name"], track);
      }
    }
#ifdef OSS_ENABLED
    else if (io["type"] == "AudioOSSOut")
    {
      auto bits = io["bits"];
      if (bits == 32)
      {
        new maolan::audio::OSSOut<int32_t>(io["name"]);
      }
      else if (bits == 16)
      {
        new maolan::audio::OSSOut<int16_t>(io["name"]);
      }
      else if (bits == 8)
      {
        new maolan::audio::OSSOut<int8_t>(io["name"]);
      }
    }
    else if (io["type"] == "AudioOSSIn")
    {
      auto bits = io["bits"];
      if (bits == 32)
      {
        new maolan::audio::OSSIn<int32_t>(io["name"]);
      }
      else if (bits == 16)
      {
        new maolan::audio::OSSIn<int16_t>(io["name"]);
      }
      else if (bits == 8)
      {
        new maolan::audio::OSSIn<int8_t>(io["name"]);
      }
    }
#endif
  }
  for (const auto &c : result["connections"])
  {
    auto fromio = IO::find(c["name"]);
    if (fromio == nullptr)
    {
      std::cerr << "Could not find IO with name " << c["name"];
      continue;
    }
    auto &fromch = c["channel"];
    for (auto &tojson : c["to"])
    {
      auto toio = IO::find(tojson["name"]);
      if (toio == nullptr)
      {
        std::cerr << "Could not find IO with name " << tojson["name"];
        continue;
      }
      auto &toch = tojson["channel"];
      if (fromio->type().substr(0, 5) == "Audio" &&
          toio->type().substr(0, 5) == "Audio")
      {
        ((audio::IO *)fromio)->connect(((audio::IO *)toio), fromch, toch);
      }
      else if (fromio->type().substr(0, 4) == "MIDI" &&
               toio->type().substr(0, 4) == "MIDI")
      {
        ((midi::IO *)fromio)->connect(((midi::IO *)toio), fromch, toch);
      }
    }
  }
  return result;
}


void Engine::setup()
{
  for (auto io = IO::begin(); io != nullptr; io = io->next())
  {
    io->setup();
  }
}


void Engine::fetch()
{
  for (auto io = IO::begin(); io != nullptr; io = io->next())
  {
    io->fetch();
  }
}


void Engine::process()
{
  for (auto io = IO::begin(); io != nullptr; io = io->next())
  {
    io->process();
  }
}


void Engine::play() { IO::play(); }
void Engine::stop() { IO::stop(); }
