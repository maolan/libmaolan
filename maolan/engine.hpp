#pragma once
#include <filesystem>
#include <nlohmann/json.hpp>
#include <vector>

#include "maolan/worker.hpp"


namespace maolan
{
class Engine
{
public:
  static void init(const int &threads = -1);
  static void fetch();
  static void play();
  static void process();
  static void quit();
  static void save();
  static void setup();
  static void stop();
  static nlohmann::json json();
  static nlohmann::json load(const std::filesystem::path &path);

protected:
  static std::vector<Worker *> _workers;
};
} // namespace maolan
