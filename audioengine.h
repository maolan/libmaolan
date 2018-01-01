#pragma once


class AudioEngine
{
  public:
    virtual int read() = 0;
    virtual void write() = 0;
};
