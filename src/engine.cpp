#include <iostream>
#include <thread>
#include "maolan/engine.hpp"
#include "maolan/io.hpp"
#include "maolan/midi/clip.hpp"
#include "maolan/plugin/lv2/plugin.hpp"


using namespace maolan;


std::vector<Worker *> Engine::_workers;


void Engine::init(const int &threads)
{
  auto maxWorkers = std::thread::hardware_concurrency();
  auto realWorkerNumber =
      threads == -1 || threads > maxWorkers ? maxWorkers : threads;
  _workers.resize(realWorkerNumber);
  for (std::size_t i = 0; i < _workers.size(); ++i)
  {
    _workers[i] = new Worker();
  }
  plugin::lv2::Plugin::init();
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
  return data;
}


void Engine::save()
{
  midi::Clip::saveAll();
  auto data = json();
  std::cout << data.dump(2) << '\n';
}


void Engine::play() { IO::play(); }
void Engine::stop() { IO::stop(); }
