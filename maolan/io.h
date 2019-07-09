/* vim: set syntax=cpp: */
#pragma once
#include <string>
#include <vector>


namespace maolan
{
  class IO
  {
    public:
      static IO * begin();

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

      void name(const std::string &);
      std::string name();


    protected:
      static IO * ios;
      static bool _stage;

      std::string _name;
      int state;
      IO * _next;
      IO * _previous;
  };
}
