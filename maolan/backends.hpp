#pragma once
#include <map>
#include <string>

#include <maolan/dynamichw.hpp>

namespace maolan {
class Backends {
public:
  static void load();
  static DynamicHW *find(const std::string &backend);

protected:
  Backends();

  static std::map<std::string, DynamicHW *> _backends;
};
} // namespace maolan
