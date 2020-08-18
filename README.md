# Maolan
DAW written in C++17

Dependencies:
* libsndfile
* pugixml
* lilv

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

All drivers are enabled by default, so you might want to disable some of them. The syntax is:
```
cmake .. -D<driver>=Off
```

### List of drivers
| Name |      Description                  |  CMake Option |
|------|:---------------------------------:|--------------:|
| OSS  | Open Sound System                 | -DOSS         |
| ALSA | Advanced Linux Sound Architecture | -DALSA        |


You can point cmake where to search for `pkg-config` .pc files and where to
put its own .pc file.
```
env PKG_CONFIG_DATA=/some/path cmake .. -DPKG_CONFIG_INSTALL_PATH=/libdata/pkgconfig
```
Note that `PKG_CONFIG_INSTALL_PATH` is relative to `PREFIX`.

By default only shared library will be built. You can turn on/off static/shared library.
```
cmake .. -DBUILD_STATIC=On -DBUILD_SHARED=On
```

## Implementation idea
The main components are:
  - maolan::IO
  - maolan::worker
  - maolan::engine

While maolan::IO is abstract class, maolan::midi::IO and maolan::audio::IO is
what every class should extend actually. In the remaining text its is just
called IO. That class has most important methods (some of them abstract):
  - setup
  - fetch
  - process
  - pull

The engine is a class with mostly static methods that binds all code together
and exposes controls: play, stop, quit.

Worker is a class that implements thread pool. One worker is one thread. As IO
has list of all it's objects, what thread pool does is takes one IO pointer at
a time and execute it's work() method. There are also 2 stages for every
`IO::work()`: fetch and process. Fetch stage calls `setup()` and `fetch()` from IO.
Setup is meant to deal with any setup track or any other IO object needs to do
before doing actual work, like muting, soloing, ... Fetch will pull() samples
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
unless it's HW IO, which calls for IO(name, false, true). That way HW is
grouped on one end of the list. If register = false, that object is not going
to be put into the list. The list I'm talking about `is IO::ios`. For example,
track needs to be registered in order for engine to call it's setup/fetch/process
methods, but Clip does not, because its methods are going to be called by a
parent Track (same for the file in a clip).

