/* vim: set syntax=cpp: */
#pragma once
#include <maolan/audio/oss.h>
#include <string>


namespace maolan
{
namespace audio
{
class OSSIn : public OSS
{
public:
  OSSIn(const std::string &device, const std::size_t &channels);

  void fetch();
  void process();
};
} // namespace audio
} // namespace maolan
