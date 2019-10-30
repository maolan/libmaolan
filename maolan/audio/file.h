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
  File(const std::string &path = "", const uint64_t &offset = 0);
  ~File();

  void fetch();
  void split();
  void process();
  void write();
  uint64_t offset();
  void offset(const uint64_t &argOffset);
  std::size_t channels() const;

  SndfileHandle audioFile();

protected:
  SndfileHandle _audioFile;
  uint64_t _offset;

private:
  float *frame;
};
} // namespace audio
} // namespace maolan
