#pragma once
#include <vector>
#include <maolan/worker.h>


namespace maolan
{
class Engine
{
  public:
    static void init(const int &threads = -1);
    static void play();
    static void stop();
    static void quit();

  protected:
    static std::vector<Worker *> _workers;
};
} // namespace maolan
