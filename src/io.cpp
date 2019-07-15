#include <iostream>
#include <maolan/io.h>

using namespace maolan;

IO * IO::ios = nullptr;
IO * last = nullptr;
bool IO::_stage = true;


IO::IO(const bool &front, const bool &reg)
  : _next{nullptr}
  , _previous{nullptr}
{
  if (reg)
  {
    if (front)
    {
      this->_next = ios;
      if (ios != nullptr)
      {
        ios->_previous = this;
      }
      ios = this;
      if (last == nullptr)
      {
        last = this;
      }
    }
    else
    {
      this->_previous = last;
      last = this;
      if (ios == nullptr)
      {
        ios = this;
      }
    }
  }
}


IO::~IO()
{
  if (this->_previous != nullptr)
  {
    this->_previous->next(this->_next);
  }
  else
  {
    ios = this->_next;
  }
  if (this->_next != nullptr)
  {
    this->_next->previous(this->_previous);
  }
  else
  {
    last = this->_previous;
  }
}


void IO::next(IO *n)
{
  _next = n;
}


IO * IO::next()
{
  return _next;
}


void IO::previous(IO *p)
{
  _previous = p;
}


IO * IO::previous()
{
  return _previous;
}


void IO::work()
{
  if (_stage)
  {
    fetch();
  }
  else
  {
    process();
  }
}


void IO::stage(const bool &s)
{
  _stage = s;
}


bool IO::stage()
{
  return _stage;
}

IO * IO::begin()
{
  return ios;
}

std::string IO::name()
{
  return _name;
}


void IO::name(const std::string &argName)
{
  _name = argName;
}
