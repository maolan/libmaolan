#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <pugixml.hpp>
#include "maolan/config.hpp"


namespace maolan
{
class IO
{
public:
  IO(const std::string &argName = "", const bool &front = false,
     const bool &reg = true);
  ~IO();

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

  virtual void setup();
  virtual void init();
  virtual void fetch() = 0;
  virtual void process() = 0;
  virtual void parent(IO *p);
  virtual nlohmann::json json();
  virtual nlohmann::json connections();
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

  void data(void *d);
  void * data();

protected:
  static bool check();

  static IO *ios;
  static IO *last;
  static IO *_current;
  static unsigned _stage;
  static bool _rec;
  static bool _playing;
  static bool _quit;
  static std::size_t _playHead;
  static std::atomic_size_t _count;
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
