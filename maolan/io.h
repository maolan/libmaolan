/* vim: set syntax=cpp: */
#pragma once
#include <string>
#include <vector>


class IO
{
  public:
    IO(bool front = true);
    ~IO();

    virtual void fetch() = 0;
    virtual void process() = 0;
    void work();

    void next(IO *);
    IO * next();
    void previous(IO *);
    IO * previous();

    void stage(const bool &s);
    bool stage();

  protected:
    static IO * ios;
    static bool _stage;

    std::string name;
    int state;
    IO * _next;
    IO * _previous;
};