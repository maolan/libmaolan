# Maolan
DAW written in C++17

Dependencies:
* libsndfile
* nlohmann-json
* lilv
* lv2

Build
```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make VERBOSE=1 -j4
```

It assumes you have 4 cores and you want `Release` build. If you're developing, you probably want `Debug`.

Building and executing tests can be done with:
```
make -j4 build-tests
make test
```

All options are disabled by default, so you might want to enable some of them. The syntax is:
```
cmake .. -D<option>=On
```

### List of drivers
| Name |      Description                  |  CMake Option |
|------|:---------------------------------:|--------------:|
| OSS  | Open Sound System                 | -DOSS         |
| ALSA | Advanced Linux Sound Architecture | -DALSA        |
| LV2  | Support for LV2 plugins           | -DLV2         |

### List of useful CMake options
|                 Option                   |                                  Description                               | 
|------------------------------------------|:--------------------------------------------------------------------------:|
| -DCMAKE_BUILD_TYPE={Release,Debug}       | Build with or without debugging symbols                                    |
| -DCMAKE_EXPORT_COMPILE_COMMANDS={On,Off} | Generate `compile_commands.json` (useful with language server like clangd) |
| -DBUILD_STATIC={On,Off}                  | Build static library                                                       |
| -DBUILD_SHARED={On,Off}                  | Build shared library (.so or .dll)                                         |


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

The `maolan::IO is` abstract class which is base for `maolan::midi::IO` and 
`maolan::audio::IO`. One of those classes is what every other class should extend. In 
the remaining text its is just called IO. That class has most important methods 
(some of them abstract):
  - setup
  - fetch
  - process
  - pull

The engine is a class with mostly static methods that binds all code together
and exposes controls: play, stop, quit.

Worker is a class that implements thread pool. One worker is one thread. As IO
has list of all it's objects, what thread pool does is takes one IO pointer at
a time and execute it's `work()` method. There are also 2 stages for every
`IO::work()`: fetch and process. Fetch stage calls `setup()` and `fetch()` from IO.
Setup is meant to deal with any setup track or any other IO object needs to do
before doing actual work, like muting, soloing, ... Fetch will `pull()` samples
(which are actually `shared_ptr` to `BufferData` and it's called `Buffer` - for MIDI
and audio) from all its connections, mix them and put them into inputs. The
process() is meant to do any processing (like LV2 or mixing of inputs, if needed)
and stores the result in the output, which is also `Buffer` class (or vector of
Buffers in case of audio where `vector.size() == number` of output channels). To
distribute output Buffer, every IO has pull() method which mostly returns
processed output.

Workers are synchronized using condition variables. The idea is that all workers
should run `setup()`/`fetch()` first and those that finish this stage should wait
for the rest of IO objects to finish that stage, too. Same thing happens for
`process()` stage, with the exception that on start of every setup/fetch stage
playhead and tempo marker are updated.

Every IO subclass' constructor calls `IO(name, front = true, register = false)`.
That means that by default, new object pointer is put to the front of the list
unless it's HW IO, which calls for `IO(name, false, true)`. That way HW is
grouped on one end of the list. If `register = false`, that object is not going
to be put into the lis: `IO::ios`. For example, `Track` needs to be registered 
in order for engine to call it's `setup`/`fetch`/`process` methods, but `Clip`
does not, because its methods are going to be called by a parent `Track`
(same for the file in a clip).

### Connections

Every class that can connect it's inputs to other class, subclasses `Connectable`.
That way code for connecting and fetching samples from connected object is grouped
in `Connectable`.
