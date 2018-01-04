#include "audioengine.h"


AudioEngine::AudioEngine()
  : position{0}
{
  inputMutexEmpty.lock();
  outputMutexEmpty.lock();
}


void AudioEngine::waitInputForSpace()
{
  while (input.size() >= 128)
  {
    inputMutex.unlock();
    inputMutexFull.lock();
    inputMutex.lock();
  }
}


void AudioEngine::waitOutputForSpace()
{
  while (output.size() >= 128)
  {
    outputMutex.unlock();
    outputMutexFull.lock();
    outputMutex.lock();
  }
}


void AudioEngine::waitInputEmpty()
{
  while (input.empty())
  {
    inputMutex.unlock();
    inputMutexEmpty.lock();
    inputMutex.lock();
  }
}


void AudioEngine::waitOutputEmpty()
{
  while (output.empty())
  {
    outputMutex.unlock();
    outputMutexEmpty.lock();
    outputMutex.lock();
  }
}
