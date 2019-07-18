# Maolan
DAW written in C++

Dependencies:
* libsndfile
* zita resampler  

For FreeBSD build in jailed environment [reggae](https://github.com/cbsd/reggae) is required.

FreeBSD build
```
git clone https://github.com/maolan/devops
cd devops
echo echo 'DEVEL_MODE=YES' > vars.mk
make devel
cd /usr/src/libmaolan
autoconf
env CFLAGS=-I/usr/local/include LDFLAGS=-L/usr/local/lib ./configure
make
```

Linux build
```
autoconf
./configure
make
```
