#pragma once
#include <mutex>
#include <deque>
#include <thread>
#include "sample.h"


using namespace std;


class AudioEngine
{
  public:
    AudioEngine();
    virtual void push(const Sample sample) = 0;
    virtual Sample & pop() = 0;
    virtual void produce() = 0;
    virtual void consume() = 0;
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
    deque<Sample> input;
    deque<Sample> output;
    bool quit;
    size_t position;
};
