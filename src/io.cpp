#include "maolan/io.hpp"
#include <iostream>


using namespace maolan;


IO *IO::ios = nullptr;
IO *IO::_last = nullptr;
IO *IO::_current = nullptr;
unsigned IO::_stage = 0;
bool IO::_rec = false;
bool IO::_playing = false;
bool IO::_quit = false;
std::size_t IO::_playHead = 0;
std::atomic_size_t IO::_count{0};
std::atomic_size_t IO::_size{0};
std::mutex IO::m;
std::condition_variable IO::cv;
std::vector<maolan::Config *> IO::_devices;


static bool firstRun = false;


enum Stage
{
  FETCH = 0,
  PROCESS,
  TOTAL
};


IO::IO(const std::string &name, const bool &reg)
    : _next{nullptr}, _previous{nullptr}, _name{name}, _data{nullptr}
{
  if (reg)
  {
    _previous = _last;
    if (_previous)
    {
      _previous->next(this);
    }
    _last = this;
    if (ios == nullptr)
    {
      ios = this;
    }
    ++_size;
    // serialize()
  }
  if (_current == nullptr)
  {
    _current = ios;
  }
}


IO::~IO()
{
  bool removed = false;
  if (_previous != nullptr)
  {
    _previous->next(_next);
    removed = true;
  }
  else
  {
    ios = _next;
  }
  if (_next != nullptr)
  {
    _next->previous(_previous);
    removed = true;
  }
  else
  {
    _last = _previous;
  }
  if (removed)
  {
    --_size;
    // serialize()
  }
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
  std::unique_lock<std::mutex> lk(m);
  cv.wait(lk, [] { return IO::check(); });
  if (_quit)
  {
    return nullptr;
  }
  ++_count;
  auto result = _current;
  if (_current != nullptr)
  {
    _current = _current->next();
  }
  lk.unlock();
  cv.notify_one();
  return result;
}


bool IO::check()
{
  if (_quit)
  {
    return true;
  }
  if (ios == nullptr)
  {
    return false;
  }
  if (!_playing)
  {
    return false;
  }
  if (_current == nullptr)
  {
    if (_count == 0)
    {
      _current = ios;
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
      return _current != nullptr;
    }
    return false;
  }
  return true;
}


void IO::play()
{
  _playing = true;
  firstRun = true;
  cv.notify_all();
}


void IO::stop()
{
  _playing = false;
  cv.notify_all();
}


void IO::quit()
{
  _quit = true;
  cv.notify_all();
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
  for (auto io = begin(); io != nullptr; io = io->next())
  {
    if (io->name() == n)
    {
      return true;
    }
  }
  return false;
}


IO *IO::find(const std::string &name)
{
  for (auto io = begin(); io != nullptr; io = io->next())
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
IO *IO::begin() { return ios; }
std::string IO::type() { return _type; }
void IO::type(const std::string &argType) { _type = argType; }
std::string IO::name() { return _name; }
void IO::name(const std::string &argName) { _name = argName; }
uint64_t IO::playHead() { return _playHead; }
void IO::playHead(const std::size_t &argPlayHead) { _playHead = argPlayHead; }
void IO::setup() {}
void IO::next(IO *n) { _next = n; }
IO *IO::next() { return _next; }
void IO::previous(IO *p) { _previous = p; }
IO *IO::previous() { return _previous; }
void *IO::data() { return _data; }
void IO::data(void *d) { _data = d; }
void IO::init() {}
nlohmann::json IO::connections() { return nullptr; }
void IO::fetch() {}
void IO::process() {}
void IO::readhw() {}
void IO::writehw() {}
