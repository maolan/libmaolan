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
