#pragma once
#include <vector>
#include <nlohmann/json.hpp>

#include "maolan/worker.hpp"


namespace maolan
{
class Engine
{
public:
  static void init(const int &threads = -1);
  static void play();
  static void stop();
  static void quit();
  static void save();
  static nlohmann::json json();

protected:
  static std::vector<Worker *> _workers;
};
} // namespace maolan
