#include <algorithm>
#include <iostream>
#include <thread>
#include <unistd.h>
#ifdef __FreeBSD__
#include <sys/rtprio.h>
#include <sys/types.h>
#endif

#include "maolan/audio/clip.hpp"
#include "maolan/audio/track.hpp"
#include "maolan/engine.hpp"
#include "maolan/io.hpp"
#include "maolan/midi/clip.hpp"
#include "maolan/midi/track.hpp"

#ifdef OSS_ENABLED
#include "maolan/oss/audio/in.hpp"
#include "maolan/oss/audio/out.hpp"
#endif

#ifdef ALSA_ENABLED
// #include "maolan/alsa/audio/in.hpp"
#include "maolan/alsa/audio/out.hpp"
#endif

#ifdef LV2_ENABLED
#include "maolan/plugin/lv2/plugin.hpp"
#endif


using namespace maolan;


scheduler::Poll *Engine::_scheduler = nullptr;
std::vector<Worker *> Engine::_workers;
static std::vector<std::string> audioNames = {
#ifdef OSS_ENABLED
    "AudioOSSIn", "AudioOSSOut",
#endif
    "AudioTrack"};
static std::vector<std::string> midiNames = {
#ifdef OSS_ENABLED
    "MidiOSSIn", "MidiOSSOut",
#endif
    "MidiTrack"};


void Engine::init(const int &threads)
{
  int rc;
#ifdef __FreeBSD__
  struct rtprio rtp;
  rtp.type = RTP_PRIO_REALTIME;
  rtp.prio = 16;
  rc = rtprio(RTP_SET, getpid(), &rtp);
  if (rc)
  {
    std::cerr << "Warning! Setting real time priority failed: ";
    std::cerr << strerror(errno) << std::endl;
  }
#endif
  if (threads != 0)
  {
    int maxWorkers = std::thread::hardware_concurrency();
    auto realWorkerNumber =
        threads == -1 || threads >= maxWorkers ? maxWorkers - 1 : threads;
    if (realWorkerNumber < 1)
    {
      realWorkerNumber = 1;
    }
    _workers.resize(realWorkerNumber);
    for (std::size_t i = 0; i < _workers.size(); ++i)
    {
      _workers[i] = new Worker();
    }
  }
  IO::initall();
#ifdef LV2_ENABLED
  plugin::lv2::Plugin::allocate();
#endif
}


nlohmann::json Engine::json()
{
  nlohmann::json data;
  data["io"] = nlohmann::json::array();
  data["connections"] = nlohmann::json::array();
  for (const auto &io : IO::all())
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


nlohmann::json Engine::load(const std::filesystem::path &path)
{
  if (!exists(path))
  {
    std::cerr << "Path " << path << " does not exist!" << std::endl;
    return nullptr;
  }

  Config::root = path;
  chdir(Config::root.string().data());
  std::ifstream session{"session.json"};
  auto result = nlohmann::json::parse(session);
  for (const auto &io : result["io"])
  {
    if (io["type"] == "AudioTrack")
    {
      auto track = new maolan::audio::Track(io["name"], io["channels"]);
      for (const auto &clipio : io["clips"])
      {
        new maolan::audio::Clip(clipio["name"], track, clipio["start"],
                                clipio["end"], clipio["offset"]);
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
        new maolan::audio::OSSOut<int32_t>(io["name"], io["device"]);
      }
      else if (bits == 16)
      {
        new maolan::audio::OSSOut<int16_t>(io["name"], io["device"]);
      }
      else if (bits == 8)
      {
        new maolan::audio::OSSOut<int8_t>(io["name"], io["device"]);
      }
    }
    else if (io["type"] == "AudioOSSIn")
    {
      auto bits = io["bits"];
      if (bits == 32)
      {
        new maolan::audio::OSSIn<int32_t>(io["name"], io["device"]);
      }
      else if (bits == 16)
      {
        new maolan::audio::OSSIn<int16_t>(io["name"], io["device"]);
      }
      else if (bits == 8)
      {
        new maolan::audio::OSSIn<int8_t>(io["name"], io["device"]);
      }
    }
#endif
#ifdef ALSA_ENABLED
    else if (io["type"] == "AudioALSAOut")
    {
      auto bits = io["bits"];
      if (bits == 32)
      {
        new maolan::audio::ALSAOut<int32_t>(io["name"], io["device"]);
      }
      else if (bits == 16)
      {
        new maolan::audio::ALSAOut<int16_t>(io["name"], io["device"]);
      }
      else if (bits == 8)
      {
        new maolan::audio::ALSAOut<int8_t>(io["name"], io["device"]);
      }
    }
    // else if (io["type"] == "AudioALSAIn")
    // {
    //   auto bits = io["bits"];
    //   if (bits == 32)
    //   {
    //     new maolan::audio::ALSAIn<int32_t>(io["name"], io["device"]);
    //   }
    //   else if (bits == 16)
    //   {
    //     new maolan::audio::ALSAIn<int16_t>(io["name"], io["device"]);
    //   }
    //   else if (bits == 8)
    //   {
    //     new maolan::audio::ALSAIn<int8_t>(io["name"], io["device"]);
    //   }
    // }
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
        ((midi::IO *)fromio)->connect((midi::IO *)toio);
      }
    }
  }
  return result;
}


void Engine::play()
{
  IO::play();
  _scheduler = new scheduler::Poll();
}


void Engine::stop()
{
  IO::stop();
  if (_scheduler != nullptr)
  {
    delete _scheduler;
    _scheduler = nullptr;
  }
}


void Engine::quit()
{
  IO::quit();
  if (_scheduler != nullptr)
  {
    delete _scheduler;
    _scheduler = nullptr;
  }
  _workers.clear();
#ifdef LV2_ENABLED
  plugin::lv2::Plugin::destroyWorld();
#endif
}
