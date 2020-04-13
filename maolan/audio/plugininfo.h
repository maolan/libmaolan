#pragma once
#include <cstddef>


namespace maolan::audio
{
class PluginPortInfo
{
  public:
    std::size_t audio;
    std::size_t control;
    std::size_t midi;
};


class PluginInfo
{
  public:
    PluginPortInfo input;
    PluginPortInfo output;
};
} // namespace maolan::audio
