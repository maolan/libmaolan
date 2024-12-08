#include <maolan/audio/output.hpp>
#include <maolan/config.hpp>

using namespace maolan::audio;

Output::Output()
    : _buffer{std::make_shared<BufferData>(Config::audioBufferSize)} {}

void Output::connect(IO *to, const size_t &ch) {
  _connections.push_back(new Connection(to, ch));
}

size_t Output::conns() { return _connections.size(); }
Buffer Output::buffer() { return _buffer; }
void Output::buffer(Buffer b) { _buffer = b; }
