#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>


using namespace std;


class Bar {
  public:
    Bar(mutex &m, const string &name = "")
      : quit{false}
      , name{name}
      , m{m}
    {
      t = thread(&Bar::process, this);
    }

    ~Bar()
    {
      quit = true;
      t.join();
      cout << name << ": Good bye, cruel world!" << endl;
    }

    void process() const
    {
      while(1)
      {
        if (quit) {return;}
        // simulate work before lock
        this_thread::sleep_for(chrono::milliseconds(100));
        lock_guard<std::mutex> lock(m);
        cout << "hello from member function " << name << endl;
        // simulate work inside lock
        this_thread::sleep_for(chrono::milliseconds(100));
      }
    }

  protected:
    bool quit;
    const string name;
    mutex &m;
    thread t;
};


