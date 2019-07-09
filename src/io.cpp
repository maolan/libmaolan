#include <iostream>
#include <maolan/io.h>


IO * IO::ios = nullptr;
IO * last = nullptr;
bool IO::_stage = true;


IO::IO(bool front)
  : _next{nullptr}
  , _previous{nullptr}
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
  std::cout << ios << ' ' << this << std::endl;
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


void IO::previous(IO *p)
{
  _previous = p;
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
