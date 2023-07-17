#pragma once
#include <string>
#include <vector>

#include "maolan/midi/hw.hpp"


namespace maolan::midi
{
class Backend
{
  public:
    virtual void create(const std::string &name, const std::string &device) = 0;
    virtual std::vector<HW *> list() = 0;
};
}
