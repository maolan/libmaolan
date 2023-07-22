# Maolan Core Library

Dependencies:
* libsndfile
* nlohmann-json
* lilv (optional)
* lv2 (optional)

Build
```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make VERBOSE=1 -j4
```

It assumes you have 4 cores and you want `Release` build. If you're developing,
you probably want `Debug`.

Development/Debug
```
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=On
```

Building and executing tests can be done with:
```
make -j4 build-tests
make test
```

### List of useful CMake options
|                 Option                   |                                  Description                               | 
|------------------------------------------|:--------------------------------------------------------------------------:|
| -DCMAKE_BUILD_TYPE={Release,Debug}       | Build with or without debugging symbols                                    |
| -DCMAKE_EXPORT_COMPILE_COMMANDS={On,Off} | Generate `compile_commands.json` (useful with language server like clangd) |


You can point cmake where to search for `pkg-config` .pc files and where to
put its own .pc file.
```
env PKG_CONFIG_DATA=/some/path cmake .. -DPKG_CONFIG_INSTALL_PATH=/libdata/pkgconfig
```
Note that `PKG_CONFIG_INSTALL_PATH` is relative to `PREFIX`.

## Implementation idea

The main components are:
  - maolan::IO
  - maolan::worker
  - maolan::engine

The `maolan::IO` is abstract class which is base for `maolan::midi::IO` and 
`maolan::audio::IO`. One of those two classes is what every other class should
extend. In the remaining text it is just called IO. That class has most
important methods (some of them abstract):
  - setup
  - fetch
  - process
  - pull

The engine is a class with mostly static methods that bind all code together
and exposes controls: play, stop, quit.

Worker is a class that implements thread pool. One worker is one thread. As IO
has list of all it's objects, what thread pool does is takes one IO pointer at
a time and execute it's `work()` method. Setup is meant to deal with any setup
track or any other IO object needs to do before doing actual work, like muting,
soloing, ... Fetch will `pull()` samples (which are actually `shared_ptr` to
`BufferData` and it's called `Buffer` - for MIDI and audio) from all its
connections, mix them and put them into inputs. The process() is meant to do
any processing (like LV2 or mixing of inputs, if needed) and stores the result
in the output, which is also `Buffer` class (or vector of Buffers in case of
audio where vector.size() == number of output channels). To distribute output
Buffer, every IO has pull() method which mostly returns processed output.

Workers are synchronized using condition variables. The idea is that all workers
should run `setup()`/`fetch()`/`process()` and when all HW IO finishes a cycle
playhead and tempo marker are updated.

Every IO subclass' constructor calls `IO(name, register = false)`. If
`register = false`, that object is not going to be put into the list:
`IO::_all`. For example, `Track` needs to be registered in order for
engine to call its `setup`/`fetch`/`process` methods, but `Clip` does not,
because its methods are going to be called by a parent `Track` (same for the
file in a clip).

To avoid latency as much as possible, Tracks are put into `IO::_ordered` list.
Actually, it is list of lists of IO objects. The idea is to process IOs which
have no input connections, then once those are finished, process IOs which only
have connections to already processed IOs and so on. The reason for such an
algorithm is so that buffer read from audio input is propagated through Tracks
and finally audio output in one cycle. If that wasn't the case Track could pull
from audio input before that input processed its samples, hence getting the
silence and increasing the latency.
