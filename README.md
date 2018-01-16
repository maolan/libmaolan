# Maolan
DAW written in C++

```
make CC=cc CXX=c++ CPPFLAGS=-isystem/usr/local/include LDFLAGS=-L/usr/local/lib git
make
make maolan-cli
env LD_PRELOAD=src/libmaolan.so src/maolan-cli ~/some.wav
```
