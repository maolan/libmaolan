#pragma once
#include <mutex>
#include <vector>
#include <thread>
#include "sample.h"


using namespace std;


class AudioEngine
{
  public:
    virtual void push(Sample &sample) = 0;
    virtual Sample pop() = 0;
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
    thread inputThread;
    vector<Sample> input;
    mutex outputMutex;
    mutex outputMutexEmpty;
    mutex outputMutexFull;
    thread outputThread;
    vector<Sample> output;
    bool quit;
    size_t position;
};
