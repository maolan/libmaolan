/* vim: set syntax=cpp: */
#pragma once
#include <string>
#include <maolan/audioio.h>
#include <maolan/ossconfig.h>


namespace maolan
{
  namespace audio
  {
    class OSS : public IO
    {
      public:
        OSS(const std::string &device);
        ~OSS();

        static std::vector<OSSConfig *> devices;

        std::size_t channels() const;

      protected:
        int *rawData;
        OSSConfig *device;
    };
  }
}
