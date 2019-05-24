CFLAGS = -I.. $(CPPFLAGS) -O2 -fPIC -DPIC
CXXFLAGS = $(CFLAGS) -std=c++11
LDFLAGS = -pthread -lsndfile
LDFLAGS_SHARED = -shared
CPPFILES != ls -1 *.cpp
OBJECTS = $(CPPFILES:.cpp=.o) $(CFILES:.c=.o)
-include ../makeinclude

.MAIN: all

all: libmaolan.so makedepend

-include makedepend

libmaolan.so: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(LDFLAGS_SHARED) -Wl,-soname,libmaolan.so -o libmaolan.so

clean:
	rm -rf $(OBJECTS) libmaolan.so maolan-cli

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

distclean: clean
	rm -rf makedepend

makedepend:
	touch makedepend

gendep: $(CXXFILES)
	$(CXX) -MM -DMAKEDEPEND $(CXXFLAGS) $(CXXFILES) >makedepend
