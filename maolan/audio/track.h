/* vim: set syntax=cpp: */
#pragma once
#include <maolan/audio/file.h>
#include <maolan/audio/input.h>
#include <maolan/audio/io.h>
#include <string>


namespace maolan
{
namespace audio
{
class Track : public IO
{
public:
  Track(const std::size_t &size);
  ~Track();

  void connect(IO *to);
  void fetch();
  std::size_t channels() const;
  void process();
  void addFile(const std::string &filePath);

protected:
  std::vector<Input> inputs;
  std::vector<File> files;
};
} // namespace audio
} // namespace maolan
