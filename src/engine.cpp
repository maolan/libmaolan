#include <iostream>
#include <thread>
#include <maolan/engine.h>
#include <maolan/io.h>


using namespace maolan;


std::vector<Worker *> Engine::_workers;


void Engine::init()
{
  auto maxWorkers = std::thread::hardware_concurrency();
  _workers.resize(maxWorkers);
  for (std::size_t i = 0; i < maxWorkers; ++i)
  {
    _workers[i] = new Worker();
  }
  std::cout << "created " << maxWorkers << " workers\n";
}


void Engine::quit()
{
  IO::quit();
  _workers.resize(0);
}

void Engine::play() { IO::play(); }
void Engine::stop() { IO::stop(); }
