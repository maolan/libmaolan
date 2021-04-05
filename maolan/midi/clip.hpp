#pragma once
#include <string>
#include "maolan/midi/buffer.hpp"
#include "maolan/midi/file.hpp"
#include "maolan/midi/io.hpp"


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

  static void saveAll();

  void load();
  virtual void fetch();
  virtual void process();
  virtual void setup();

  const std::size_t &start() const;
  const std::size_t &end() const;
  std::size_t startSample() const;
  std::size_t endSample() const;
  void start(const std::size_t &);
  void end(const std::size_t &);
  void startSample(const std::size_t &);
  void endSample(const std::size_t &);

  void next(Clip *n);
  Clip *next();
  void previous(Clip *n);
  Clip *previous();
  void parent(maolan::IO *p);

  void write(const Buffer buffer);
  void save();

protected:
  static std::vector<Clip *> clips;

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
