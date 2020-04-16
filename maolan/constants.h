#pragma once
#include <limits>

namespace maolan
{
const int maxInt = std::numeric_limits<int>::max();
const int minInt = std::numeric_limits<int>::min();
const float floatMaxInt = float(maxInt);
const float floatMinInt = float(minInt);

namespace audio
{
const int defaultFrag = 16;
} // namespace audio
} // namespace maolan
