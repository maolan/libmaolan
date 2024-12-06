#include <maolan/scheduler/base.hpp>

using namespace maolan::scheduler;

BaseScheduler::BaseScheduler() {}
BaseScheduler::~BaseScheduler() { join(); }
void BaseScheduler::join() { _thread.join(); }
