#pragma once
#include <mutex>
#include <map>
#include <thread>
#include <deque>


using namespace std;


class AudioEngine
{
  public:
    AudioEngine();
    virtual void push(const int channel) = 0;
    virtual int pop(const int channel) = 0;
    virtual void produce() = 0;
    virtual void consume() = 0;
    virtual void sync() = 0;
    void waitInputForSpace();
    void waitInputEmpty();
    void waitOutputForSpace();
    void waitOutputEmpty();

  protected:
    mutex inputMutex;
    mutex inputMutexEmpty;
    mutex inputMutexFull;
    mutex outputMutex;
    mutex outputMutexEmpty;
    mutex outputMutexFull;
    thread outputThread;
    thread inputThread;
    map<int, deque<int>> input;
    map<int, deque<int>> output;
    bool quit;
    size_t position;
};
