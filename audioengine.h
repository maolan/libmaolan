#pragma once


class AudioEngine
{
  public:
    virtual void push() = 0;
    virtual void pop() = 0;
    virtual void produce() = 0;
    virtual void consume() = 0;
};
