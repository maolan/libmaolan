#pragma once
#include <sndio.h>
#include <nlohmann/json.hpp>

#include "maolan/audio/hw.hpp"


namespace maolan::audio
{
class SNDIO : public HW
{
public:
  SNDIO(const std::string &name, const std::string &device, const unsigned int &mode, const size_t &sampleSize);
  ~SNDIO();

  virtual size_t channels() const;
  virtual nlohmann::json json();

protected:
  struct sio_hdl *_handle;
  struct sio_par *_parameters;
  int8_t *_bytes;
};
}
