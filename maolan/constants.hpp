#pragma once
#include <limits>

namespace maolan {
const int32_t maxInt = std::numeric_limits<int32_t>::max();
const int32_t minInt = std::numeric_limits<int32_t>::min();
const float floatMaxInt = static_cast<float>(maxInt);
const float floatMinInt = static_cast<float>(minInt);

namespace audio {
const int defaultFrag = 10;
} // namespace audio
} // namespace maolan
