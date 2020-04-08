#include <vector>
#include <string>
#include <iostream>
#include <lilv/lilv.h>


namespace maolan::audio
{
class Author
{
  public:
    std::string name;
    std::string homepage;
    std::string email;
};


class Plugin
{
  public:
    Plugin(const std::string &argUri);
    ~Plugin();

    static void destroyWorld();

    void uri(const LilvNode *argUri);
    const LilvNode * uri() const;
    const std::string identifier() const;
    const std::string name() const;
    const Author author() const;
    void print() const;

  protected:
    static LilvWorld *world;
    static LilvPlugins *plugins;

    const LilvNode *_uri;
    const std::string _identifier;
    std::string _name;
    LilvPlugin *rawPlugin;
    Author _author;
};
} // namespace maolan::audio
