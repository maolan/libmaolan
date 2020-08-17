#pragma once
#include <maolan/midi/buffer.h>
#include <maolan/midi/file.h>
#include <maolan/midi/io.h>
#include <string>


namespace maolan::midi
{
class Track;
class Clip : public IO
{
public:
  Clip(const std::string &name, Track *parent);
  Clip(const std::string &name, const std::size_t &start,
       const std::size_t &end, const std::size_t &offset, Track *parent);
  ~Clip();

  void load();
  virtual void fetch();
  virtual void process();
  virtual void setup();

  const std::size_t &start() const;
  const std::size_t &end() const;
  const std::size_t startSample() const;
  const std::size_t endSample() const;
  void start(const std::size_t &);
  void end(const std::size_t &);
  void startSample(const std::size_t &);
  void endSample(const std::size_t &);

  void next(Clip *n);
  Clip *next();
  void previous(Clip *n);
  Clip *previous();
  void parent(maolan::IO *p);

protected:
  std::size_t _offset = 0;
  std::size_t _start = 0;
  std::size_t _end = 0;
  std::string _name;
  Buffer data;
  Buffer last;
  Buffer current;
  File file;
  Track *_parent;
  Clip *_next;
  Clip *_previous;
};
} // namespace maolan::midi
