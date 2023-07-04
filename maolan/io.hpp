#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "maolan/config.hpp"

namespace maolan
{
class IO
{
public:
  IO(const std::string &argName = "", const bool &reg = false);
  virtual ~IO();

  static void rec(bool record);
  static bool rec();
  static void play();
  static void stop();
  static void quit();
  static void playHead(const std::size_t &argPlayHead);
  static std::size_t playHead();
  static IO *task();
  static IO *begin();
  static Config *devices();
  static IO *find(const std::string &name);
  static void serialize();

  virtual void setup();
  virtual void init();
  virtual void fetch() = 0;
  virtual void process() = 0;
  virtual void parent(IO *p);
  virtual nlohmann::json json();
  virtual nlohmann::json connections();
  virtual void readhw();
  virtual void writehw();
  virtual bool leaf();

  void work();
  void next(IO *);
  IO *next();
  void previous(IO *);
  IO *previous();
  void stage(const bool &s);
  bool stage();
  void type(const std::string &);
  std::string type();
  void name(const std::string &);
  std::string name();
  bool exists(const std::string_view);

  void data(void *d);
  void *data();

protected:
  static bool check();

  static IO *_all;
  static IO *_last;
  static IO *_current;
  static unsigned _stage;
  static bool _rec;
  static bool _playing;
  static bool _quit;
  static std::size_t _playHead;
  static std::atomic_size_t _count;
  static std::atomic_size_t _size;
  static std::mutex m;
  static std::condition_variable cv;
  static std::vector<Config *> _devices;

  IO *_next;
  IO *_previous;
  std::string _type;
  std::string _name;
  void *_data;
};
} // namespace maolan
