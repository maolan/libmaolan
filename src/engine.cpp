#include <maolan/engine.h>
#include <maolan/io.h>
#include <maolan/midi/clip.h>
#include <maolan/plugin/lv2/plugin.h>
#include <thread>


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


void Engine::save() { midi::Clip::saveAll(); }
void Engine::play() { IO::play(); }
void Engine::stop() { IO::stop(); }
