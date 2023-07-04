#include <stdexcept>
#include <sys/event.h>

#include "maolan/oss/io.hpp"


using namespace maolan::audio::oss;


std::vector<IO *> IO::ios;
int IO::kq = -1;


IO::IO(const std::string &device)
  : maolan::audio::IO{device}
{
  ios.push_back(this);

  if (kq == -1)
  {
    kq = kqueue();
    if (kq == -1) {
      throw std::runtime_error("kqueue() failed");
    }

    // EV_SET(event, config.fd, EVFILT_READ, EV_ADD | EV_CLEAR, NOTE_READ, 0, NULL);
    // ret = kevent(kq, event, 1, NULL, 0, NULL);
    // if (ret == -1) {
    //   throw std::runtime_error("kevent register");
    // }
  }


  // ret = kevent(kq, NULL, 0, &targetEvent, 1, NULL);
  // if (ret == -1) {
  //   err(EXIT_FAILURE, "kevent wait");
  // } else if (ret > 0) {
  //   if (targetEvent.flags & EV_ERROR) {
  //     errx(EXIT_FAILURE, "Event error: %s",
  //         strerror(targetEvent.data));
  //     return;
  //   }
  //   if (targetEvent.ident == config.fd) {
  //     handle_audio(&config);
  //   } else if (targetEvent.ident == midi_config.fd) {
  //     handle_midi(&midi_config);
  //   }
  // }
}


IO::~IO()
{
  ios.erase(std::remove(ios.begin(), ios.end(), this), ios.end()); 
}

maolan::IO * IO::wait()
{
  // kqueue/kevent stuff
  return nullptr;
}
