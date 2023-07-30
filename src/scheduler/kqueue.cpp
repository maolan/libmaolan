#include <iostream>
#include <sys/event.h>

#include <maolan/audio/hw.hpp>
#include <maolan/midi/hw.hpp>
#include <maolan/scheduler/kqueue.hpp>

using namespace maolan::scheduler;

static int kq;

KQueue::KQueue() {
  const auto &audiohw = audio::HW::all();
  const auto &midihw = midi::HW::all();
  struct kevent event;
  int rc;

  kq = kqueue();
  if (kq == -1) {
    std::cerr << "kqueue() failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  for (const auto &hw : audiohw) {
    std::cerr << "device: " << hw->name() << ' ' << hw->fd() << std::endl;
    EV_SET(&event, hw->fd(), EVFILT_READ, EV_ADD | EV_CLEAR, NOTE_READ, 0,
           nullptr);
    rc = kevent(kq, &event, 1, nullptr, 0, nullptr);
    if (rc == -1) {
      std::cerr << "kevent() failed: " << strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }
    hw->readhw();
    hw->process();
  }
  for (const auto &hw : midihw) {
    EV_SET(&event, hw->fd(), EVFILT_READ, EV_ADD | EV_CLEAR, NOTE_READ, 0,
           nullptr);
    rc = kevent(kq, &event, 1, nullptr, 0, nullptr);
    if (rc == -1) {
      std::cerr << "kevent() failed: " << strerror(errno) << std::endl;
      std::cerr << "device: " << hw->name() << ' ' << hw->fd() << std::endl;
      exit(EXIT_FAILURE);
    }
    hw->readhw();
    hw->process();
  }
  _thread = std::thread(&KQueue::_process, this);
}

maolan::IO *KQueue::wait() {
  int rc;
  struct kevent event;

  rc = kevent(kq, NULL, 0, &event, 1, NULL);
  if (rc == -1) {
    std::cerr << "kevent wait" << std::endl;
    exit(EXIT_FAILURE);
  } else if (rc > 0) {
    if (event.flags & EV_ERROR) {
      std::cerr << "Event errors: " << strerror(event.data) << std::endl;
      return nullptr;
    }
    return (IO *)event.udata;
  }
  return nullptr;
}

void KQueue::_process() {
  size_t pindex = 0;
  const auto &audiohw = audio::HW::all();

  while (IO::playing() && !IO::quitting()) {
    auto *hwio = wait();
    hwio->readhw();
    hwio->writehw();
    if (std::find(audiohw.begin(), audiohw.end(), hwio) != audiohw.end()) {
      ++pindex;
      if (pindex >= audiohw.size()) {
        pindex = 0;
        IO::tick();
      }
    }
  }
}

KQueue::~KQueue() { _thread.join(); }
