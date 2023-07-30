#pragma once
#include <memory>
#include <vector>

namespace maolan::audio {
typedef std::vector<float> BufferData;
typedef std::shared_ptr<BufferData> Buffer;
} // namespace maolan::audio
