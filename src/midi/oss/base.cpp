#include <cstring>
#include <fcntl.h>
#include <maolan/midi/event.h>
#include <maolan/midi/oss/base.h>
#include <unistd.h>


using namespace maolan::midi;


OSSMIDI::OSSMIDI(const std::string &deviceName)
  : MIDIIO(deviceName, true, true)
  , device{nullptr}
{

  data = std::make_shared<BufferData>();
  bool found = false;
  for (const auto iter : devices)
  {
    if (iter->name == "OSSMIDI" && iter->device == deviceName)
    {
      found = true;
      device = (OSSMIDIConfig *)iter;
      ++(device->count);
      break;
    }
  }
  if (!found)
  {
    device = new OSSMIDIConfig;
    device->device = deviceName;
    if ((device->fd = open(deviceName.data(), O_RDWR | O_NONBLOCK, 0)) == -1)
    {
      std::cerr << deviceName.data() << ' ' << std::strerror(errno) << '\n';
    }
    devices.emplace(devices.begin(), device);
  }
}


OSSMIDI::~OSSMIDI()
{
  --(device->count);
  if (device->count < 1)
  {
    close(device->fd);
  }
}
