/* vim: set syntax=cpp: */
#pragma once
#include <pugixml.hpp>
#include <string>
#include <vector>


namespace maolan
{
class IO
{
public:
  static IO *begin();

  IO(const std::string &argName = "", const bool &front = true,
     const bool &reg = true);
  ~IO();

  virtual void fetch() = 0;
  virtual void process() = 0;
  virtual void parent(IO *p);

  void work();

  void next(IO *);
  IO *next();
  void previous(IO *);
  IO *previous();

  void rec(bool record);
  bool rec();

  void stage(const bool &s);
  bool stage();

  void type(const std::string &);
  std::string type();

  void name(const std::string &);
  std::string name();

  virtual void setup();
  static void playHead(const uint64_t &argPlayHead);
  static uint64_t playHead();

  static IO *loadFromXml(pugi::xml_node *n);

protected:
  static IO *ios;
  static bool _stage;
  static bool _rec;
  static uint64_t _playHead;

  std::string _type;
  std::string _name;
  int state;
  IO *_next;
  IO *_previous;
};
} // namespace maolan
