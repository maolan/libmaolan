#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include <maolan/config.hpp>

namespace maolan {
class IO;
typedef std::vector<IO *> io_t;

class IO {
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
  static Config *devices();
  static void initall();
  static IO *find(const std::string &name);
  static const io_t all();
  static bool playing();
  static bool quitting();
  static void tick();
  static void drain();

  virtual void setup();
  virtual void init();
  virtual void fetch() = 0;
  virtual void process() = 0;
  virtual void parent(IO *p);
  virtual nlohmann::json json();
  virtual nlohmann::json connections();
  virtual void readhw();
  virtual void writehw();
  virtual bool processed() const;
  virtual void processed(const bool &p);
  virtual bool ready() const;
  virtual bool processing() const;
  virtual void processing(const bool &p);

  void work();
  void type(const std::string &);
  std::string type();
  void name(const std::string &);
  std::string name();
  bool exists(const std::string_view);

  void data(void *d);
  void *data();

protected:
  static bool check();
  static bool allready();

  static bool _rec;
  static bool _playing;
  static bool _quit;
  static std::atomic_size_t _playHead;
  static std::atomic_bool _all_processed;
  static std::mutex _m;
  static std::condition_variable _cv;
  static io_t _all;
  static std::atomic_size_t _active;

  std::string _type;
  std::string _name;
  void *_data;
  bool _processed;
  bool _processing;
};
} // namespace maolan
