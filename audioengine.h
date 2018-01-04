#pragma once
#include <mutex>
#include <map>
#include <thread>
#include <deque>
#include "sample.h"


using namespace std;


class AudioEngine
{
  public:
    AudioEngine();
    virtual void push(const Sample sample) = 0;
    virtual Sample & pop(const int channel) = 0;
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
    map<int, deque<Sample>> input;
    map<int, deque<Sample>> output;
    bool quit;
    size_t position;
};
