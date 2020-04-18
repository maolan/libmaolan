#pragma once
#include <string>
#include <vector>
#include <sndfile.hh>
#include <maolan/frame.h>
#include <maolan/audio/io.h>


namespace maolan::audio
{
class File : public IO
{
public:
  File(const std::size_t &channels);
  File(const std::string &path, const uint64_t &offset = 0);
  ~File();

  virtual void fetch();
  virtual void split();
  virtual void process();
  void write(const Frame &frame);
  void write(const Frame * const frame);
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
