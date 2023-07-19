#include <iostream>
#include <sndio.h>


int main()
{
  struct sio_hdl *handler;
  struct sio_par *parameters = new sio_par();
  sio_initpar(parameters);
  parameters->rate = 48000;
  parameters->appbufsz = 4096;

  handler = sio_open("default", SIO_PLAY, 0);
  sio_setpar(handler, parameters);
  sio_getpar(handler, parameters);
  std::cout << parameters->appbufsz << ' ' << parameters->bufsz << std::endl;
  sio_start(handler);

  sio_stop(handler);
  delete parameters;
  return 0;
}
