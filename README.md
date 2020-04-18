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
