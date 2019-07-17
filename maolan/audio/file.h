/* vim: set syntax=cpp: */
#pragma once
#include <maolan/audio/io.h>
#include <sndfile.hh>
#include <string>
#include <vector>


namespace maolan
{
namespace audio
{
class File : public IO
{
public:
  File(const std::string &path = "");
  ~File();

  void fetch();
  void split();
  void process();
  std::size_t channels() const;

protected:
  SndfileHandle audioFile;

private:
  float *frame;
};
} // namespace audio
} // namespace maolan
