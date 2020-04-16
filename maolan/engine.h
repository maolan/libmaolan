#pragma once
#include <maolan/worker.h>


namespace maolan
{
class Engine
{
  public:
    static void init();
    static void play();
    static void stop();
    static void quit();

  protected:
    static std::vector<Worker *> _workers;
};
} // namespace maolan
