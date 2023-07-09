#include "maolan/io.hpp"
#include <iostream>


using namespace maolan;


bool IO::_rec = false;
bool IO::_playing = false;
bool IO::_quit = false;
std::size_t IO::_playHead = 0;
std::atomic_size_t IO::_count{0};
std::atomic_size_t IO::_stage{0};
std::atomic_size_t ioindex{0};
std::mutex IO::_m;
std::condition_variable IO::_cv;
std::vector<maolan::Config *> IO::_devices;
std::vector<IO *> IO::_all;


static bool firstRun = false;


enum Stage
{
  FETCH = 0,
  PROCESS,
  TOTAL
};


IO::IO(const std::string &name, const bool &reg) : _name{name}, _data{nullptr}
{
  if (reg)
  {
    _all.push_back(this);
  }
}


IO::~IO()
{
  _all.erase(std::remove(_all.begin(), _all.end(), this), _all.end());
}


void IO::work()
{
  if (_stage == FETCH)
  {
    setup();
    fetch();
  }
  else if (_stage == PROCESS)
  {
    process();
  }
  --_count;
}


IO *IO::task()
{
  std::unique_lock<std::mutex> lk(_m);
  _cv.wait(lk, IO::check);
  if (_quit)
  {
    lk.unlock();
    return nullptr;
  }
  auto result = _all[ioindex];
  ++_count;
  ++ioindex;
  lk.unlock();
  _cv.notify_one();
  return result;
}


bool IO::check()
{
  if (_quit)
  {
    return true;
  }
  if (_all.size() == 0)
  {
    return false;
  }
  if (!_playing)
  {
    return false;
  }
  if (ioindex >= _all.size())
  {
    if (_count == 0)
    {
      ioindex = 0;
      _stage = ++_stage % TOTAL;
      if (_stage == FETCH)
      {
        if (firstRun)
        {
          firstRun = false;
        }
        else
        {
          _playHead += Config::audioBufferSize;
        }
        if (Config::tempoIndex + 1 < Config::tempos.size())
        {
          auto tempo = Config::tempos[Config::tempoIndex];
          while (Config::tempoIndex < Config::tempos.size() &&
                 _playHead <= tempo.time)
          {
            ++Config::tempoIndex;
            tempo = Config::tempos[Config::tempoIndex];
          }
        }
      }
      return _all.size() != 0;
    }
    return false;
  }
  return true;
}


void IO::play()
{
  _playing = true;
  firstRun = true;
  _cv.notify_all();
}


void IO::stop()
{
  _playing = false;
  _cv.notify_all();
}


void IO::quit()
{
  _quit = true;
  _cv.notify_all();
}


nlohmann::json IO::json()
{
  nlohmann::json data;
  data["name"] = _name;
  data["type"] = _type;
  return data;
}


bool IO::exists(std::string_view n)
{
  for (const auto &io : _all)
  {
    if (io->name() == n)
    {
      return true;
    }
  }
  return false;
}


void IO::initall()
{
  for (const auto &io : _all)
  {
    io->init();
  }
}


IO *IO::find(const std::string &name)
{
  for (const auto &io : _all)
  {
    if (io->name() == name)
    {
      return io;
    }
  }
  return nullptr;
}


void IO::parent(IO *) {}
void IO::rec(bool record) { _rec = record; }
bool IO::rec() { return _rec; }
void IO::stage(const bool &s) { _stage = s; }
bool IO::stage() { return _stage; }
std::string IO::type() { return _type; }
void IO::type(const std::string &argType) { _type = argType; }
std::string IO::name() { return _name; }
void IO::name(const std::string &argName) { _name = argName; }
uint64_t IO::playHead() { return _playHead; }
void IO::playHead(const std::size_t &argPlayHead) { _playHead = argPlayHead; }
void IO::setup() {}
void *IO::data() { return _data; }
void IO::data(void *d) { _data = d; }
void IO::init() {}
nlohmann::json IO::connections() { return nullptr; }
void IO::fetch() {}
void IO::process() {}
void IO::readhw() {}
void IO::writehw() {}
const std::vector<IO *> IO::all() { return _all; }
bool IO::playing() { return _playing; }
bool IO::quitting() { return _quit; }
