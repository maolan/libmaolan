#pragma once
#include <vector>
#include <string>
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
  static nlohmann::json load(const std::string &path);

protected:
  static std::vector<Worker *> _workers;
};
} // namespace maolan
