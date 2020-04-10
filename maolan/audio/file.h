#pragma once
#include <maolan/audio/io.h>
#include <sndfile.hh>
#include <string>
#include <vector>


namespace maolan::audio
{
class File : public IO
{
public:
  File(const std::size_t &channels);
  File(const std::string &path, const uint64_t &offset = 0);
  ~File();

  void fetch();
  void split();
  void process();
  void write(const std::vector<Buffer> &frame);
  uint64_t offset();
  void offset(const uint64_t &argOffset);
  std::size_t channels() const;

  SndfileHandle audioFile();

protected:
  SndfileHandle _audioFile;
  uint64_t _offset;
  bool recording;

private:
  float *frame = nullptr;
};
} // namespace maolan::audio
