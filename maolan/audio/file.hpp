#pragma once
#include <filesystem>
#include <sndfile.hh>
#include <string>
#include <vector>

#include <maolan/audio/io.hpp>
#include <maolan/frame.hpp>

namespace maolan::audio {
class File : public audio::IO {
public:
  File(const size_t &channels);
  File(const std::filesystem::path &path, const uint64_t &offset = 0);
  ~File();

  virtual void fetch();
  virtual void init();
  virtual void split();
  virtual void process();
  void write(const Frame &frame);
  void write(const Frame *const frame);
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
